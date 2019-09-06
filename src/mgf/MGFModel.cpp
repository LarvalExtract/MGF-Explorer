#include "MGFModel.h"
#include "MGFArchive.h"

#include <pugixml-1.9/src/pugixml.hpp>

#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4x4 CreateTransformFromXml(const pugi::xml_node& node);

MGFModel::MGFModel(const MGFTreeNode& node)
{
	// Copy MGMODEL XML contents to memory
	std::vector<char> xmlBuffer(node.FileLength());
	node.archive.FileStream().seekg(node.FileOffset(), std::ios::beg);
	node.archive.FileStream().read(xmlBuffer.data(), node.FileLength());

	pugi::xml_document doc;
	doc.load_buffer(xmlBuffer.data(), xmlBuffer.size());

	pugi::xml_node xmlres;
	xmlres = doc.first_child();

	std::string nodeType;
	nodeType.reserve(40);
	for (pugi::xml_node it = xmlres.first_child(); !it.empty(); it = it.next_sibling())
	{
		nodeType = it.name();
		std::string name = it.attribute("name").as_string();

		if (nodeType.find("material") != std::string::npos)
		{
			if (nodeType.find("base") != std::string::npos)
				materials.insert(std::make_pair(name, new MGFMaterialBase(node, it)));
			//else if (nodeType.find("animated") != std::string::npos) { if (materials.find(name) == materials.end()) materials.insert(std::make_pair(name, new MGFMaterialBase(node, it))); }
			else if (nodeType.find("color_shift") != std::string::npos)
				materials.insert(std::make_pair(name, new MGFMaterialBase(node, it)));
			else if (nodeType.find("complex") != std::string::npos)
				materials.insert(std::make_pair(name, new MGFMaterialComplex(node, it)));
			else if (nodeType.find("detail") != std::string::npos)
				materials.insert(std::make_pair(name, new MGFMaterialBase(node, it)));
			//else if (nodeType.find("distort") != std::string::npos) { if (materials.find(name) == materials.end()) materials.insert(std::make_pair(name, new MGFMaterialBase(node, it))); }
			//else if (nodeType.find("env") != std::string::npos) { if (materials.find(name) == materials.end()) materials.insert(std::make_pair(name, new MGFMaterialBase(node, it))); }
			//else if (nodeType.find("solid") != std::string::npos) { if (materials.find(name) == materials.end()) materials.insert(std::make_pair(name, new MGFMaterialBase(node, it))); }
			//else if (nodeType.find("specular_alpha") != std::string::npos) { if (materials.find(name) == materials.end()) materials.insert(std::make_pair(name, new MGFMaterialBase(node, it))); }
			else if (nodeType.find("specular_mask") != std::string::npos)
				materials.insert(std::make_pair(name, new MGFMaterialSpecularMask(node, it)));
			else
				materials.insert(std::make_pair(name, new MGFMaterial()));
		}
		else if (nodeType.find("meshxml") != std::string::npos)
		{
			const MGFTreeNode& vbNode = *node.GetNamedSibling(node.Name().Lower().ToStdString() + '{' + it.attribute("vertices").as_string() + '}');
			const MGFTreeNode& ibNode = *node.GetNamedSibling(node.Name().Lower().ToStdString() + '{' + it.attribute("indices").as_string() + '}');
			const MGFMaterial* material = (materials.find(it.attribute("material").as_string()) != materials.end()) ? materials.at(it.attribute("material").as_string()) : nullptr;
			
			meshNames.push_back(name);
			meshes.emplace(name, MGFMesh(vbNode, ibNode, it, material));
		}
	}

	TraverseXmlScene(xmlres.last_child(), CreateTransformFromXml(xmlres.last_child()));
}

MGFModel::~MGFModel()
{
	for (auto mat : materials)
		delete mat.second;
}

unsigned int MGFModel::GetColumnCount() const
{
	return 4;
}

wxString MGFModel::GetColumnType(unsigned int col) const
{
	return "string";
}

unsigned int MGFModel::GetCount() const
{
	return meshNames.size();
}

void MGFModel::GetValueByRow(wxVariant& variant, unsigned row, unsigned col) const
{
	switch (col)
	{
	case 0: variant = meshNames[row]; break;
	case 1: variant = wxString::Format("%i", meshes.at(meshNames[row].ToStdString()).numVertices); break;
	case 2: variant = wxString::Format("%i", meshes.at(meshNames[row].ToStdString()).stride); break;
	case 3: variant = wxString::Format("%02x", meshes.at(meshNames[row].ToStdString()).flags); break;
	}
}

bool MGFModel::SetValueByRow(const wxVariant& variant, unsigned row, unsigned col)
{
	return false;
}

glm::mat4x4 CreateTransformFromXml(const pugi::xml_node& node)
{
	auto XmlAttributeToVec3 = [](const pugi::xml_attribute& attrib) -> glm::vec3
	{
		if (attrib.empty())
			return glm::vec3(1.0f);

		glm::vec3 result;
		std::stringstream ss(attrib.as_string());
		std::string value;

		for (int i = 0; std::getline(ss, value, ','); i++)
			result[i] = std::stof(value);

		return result;
	};

	glm::mat4x4 id(1.0f);

	return
		glm::translate(id, XmlAttributeToVec3(node.attribute("position"))) *
		glm::rotate(id, node.attribute("rot_angle").as_float(), XmlAttributeToVec3(node.attribute("rot_axis"))) *
		glm::scale(id, XmlAttributeToVec3(node.attribute("scale")));
}

void MGFModel::TraverseXmlScene(const pugi::xml_node& node, const glm::mat4x4& parentTransform)
{
	glm::mat4x4 localTransform(1.0f);

	for (auto it = node.begin(); it != node.end(); it++)
	{
		if (it->name()[0] == 'n')
			localTransform = CreateTransformFromXml(*it);
		else if (std::strcmp(it->name(), "mesh") == 0)
			meshes.at(it->attribute("name").as_string()).transform = parentTransform;

		TraverseXmlScene(*it, parentTransform * localTransform);
	}
}
