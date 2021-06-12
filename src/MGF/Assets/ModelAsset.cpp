#include "ModelAsset.h"
#include "Factories/MeshFactory.h"
#include "Factories/MaterialFactory.h"

#include "Utilities/ContextProvider/ServiceProvider.h"
#include "MGF/AssetManager.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

using namespace MGF::Asset;
using namespace MGF::Factories;

ModelAsset::ModelAsset(const MGF::File& file) :
	AssetBase(file, MGF::Asset::EAssetType::Model),
	SceneManager(*ServiceProvider::Inject<Ogre::SceneManager>()),
	Nodes{std::make_shared<ModelViewer::Models::NodeTree>()},
	Animations{std::make_shared<ModelViewer::Models::AnimationTableModel>()},
	Meshes{std::make_shared<ModelViewer::Models::MeshTable>()},
	Materials{std::make_shared<ModelViewer::Models::MaterialTable>()}
{
	if (file.FileType() == MGF::EFileType::mgmodel)
	{
		ParseMgmodelXml();
	}
	else if (file.FileType() == MGF::EFileType::node)
	{
		ParseNodeTxt();
	}
}

void ModelAsset::ParseMgmodelXml()
{
	std::vector<char> buffer(FileRef.FileLength());
	FileRef.Read(buffer.data());

	pugi::xml_document document;
	document.load_string(buffer.data());
	const auto& root = document.root().first_child();
	auto node = root.first_child();

	std::unordered_map<std::string, Ogre::MaterialPtr> materials;
	std::unordered_map<std::string, Ogre::MeshPtr> meshes;

	std::string_view name(node.name());
	// Load materials from XML
	for (; name.find("mat") != std::string::npos; node = node.next_sibling(), name = node.name())
	{
		std::string matName(node.attribute("name").as_string());

		if (materials.find(matName) == materials.end())
		{
			auto& matDef = Materials->emplace_back(MGF::Factories::MaterialFactory::CreateMaterialDefinition(node));
			auto material = MGF::Factories::MaterialFactory::Create(matDef, FileRef);
			materials.insert(std::make_pair(matName, material));
		}
	}

	// Load meshes from XML
	for (; name.find("mesh") != std::string::npos; node = node.next_sibling(), name = node.name())
	{
		std::string meshName(node.attribute("name").as_string());

		if (meshes.find(meshName) == meshes.end())
		{
			auto& meshDef = Meshes->emplace_back(MeshFactory::CreateMeshDefinition(node));

			auto mesh = MeshFactory::Create(meshDef, FileRef);

			auto material = materials.at(node.attribute("material").as_string());
			mesh->getSubMesh(0)->setMaterial(material);

			auto technique = material->getTechnique(0);
			auto pass = technique->getPass(0);
			if (pass->getTextureUnitStates().size() > 0)
			{
				pass->getTextureUnitState(0)->setTextureScale(meshDef.texCoordScale, meshDef.texCoordScale);
				pass->getTextureUnitState(0)->setTextureScroll(meshDef.texCoordOffset, meshDef.texCoordOffset);
			}

			meshes.insert(std::make_pair(meshName, mesh));
		}
	}

	Nodes->RootNode = new Model::Node;
	Nodes->RootNode->sceneNode = SceneManager.createSceneNode();
	for (; name.find("node") != std::string::npos; node = node.next_sibling(), name = node.name())
	{
		Nodes->RootNode->children.push_back(CreateSceneNode(Nodes->RootNode, node, meshes));
	}
}

void ModelAsset::ParseNodeTxt()
{
	std::string buf;
	FileRef.LoadBuffer(buf);
	std::string_view buf_view(buf);

	std::size_t chunkStart = buf_view.find('[', 0);
	std::size_t chunkEnd = buf_view.find('[', chunkStart + 1);
	std::string_view chunk;

	auto ParseNodeSection = [&]()
	{
		chunkStart = chunkEnd;
		chunkEnd = buf_view.find('[', chunkEnd + 1);
		chunk = buf_view.substr(chunkStart, chunkEnd - chunkStart);

		return ConfigSection(chunk);
	};

	Nodes->RootNode = CreateSceneNode(nullptr, ParseNodeSection);
}

Ogre::Vector3 StrToVector(const std::string_view);
Ogre::Quaternion StrToQuat(const std::string_view, const std::string_view);

Model::Node* ModelAsset::CreateSceneNode(Model::Node* parent, const std::function<ConfigSection()>& func)
{
	auto vars = func();

	if (const auto& nodeType = vars["type"]; nodeType == "ANIMNODE")
	{
		auto& rm = *ServiceProvider::Inject<AssetManager>();
		auto childNodeFile = FileRef.FindRelativeItem(vars["child"].c_str());
		auto childNodeAsset = static_cast<ModelAsset*>(rm.Get(*childNodeFile).get());

		Nodes = childNodeAsset->Nodes;
		Meshes = childNodeAsset->Meshes;
		Materials = childNodeAsset->Materials;

		int num_animations = std::stoi(vars["num_animations"]);
		for (int i = 0; i < num_animations; i++)
		{
			auto anim = func();

			Model::Animation animDef;
			animDef.name = std::move(anim["name"]);
			animDef.animation = std::move(anim["animation"]);
			animDef.synch_point = std::stof(anim["synch_point"]);
			animDef.loop_count = std::stoi(anim["loop_count"]);
			//animDef.blend_out_duration = std::stof(anim["blend_out_duration"]);
			animDef.primary = (anim["primary"][0] == 't');

			Animations->push_back(std::move(animDef));
		}

		// TODO: Parse animations
		return Nodes->RootNode; // there won't be anymore nodes, exit the function
	}
	else
	{
		auto node = new Model::Node;
		node->name = vars.Name();
		node->type = nodeType;
		node->parent = parent;
		node->sceneNode = parent ? parent->sceneNode->createChildSceneNode() : SceneManager.createSceneNode();
		node->sceneNode->setPosition(StrToVector(vars["position"]));
		node->sceneNode->setOrientation(StrToQuat(vars["rot_axis"], vars["rot_angle"]));
		node->sceneNode->setScale(StrToVector(vars["scale"]));

		if (nodeType == "3DOBJECT" || nodeType == "SKIN")
		{
			const auto meshFile = FileRef.FindRelativeItem(vars["mesh"].data());
			const auto& meshDef = Meshes->emplace_back(MGF::Factories::MeshFactory::CreateMeshDefinition(*meshFile));
			const auto mesh = MGF::Factories::MeshFactory::Create(meshDef, *meshFile);

			const auto materialFile = meshFile->FindRelativeItem(meshDef.materialPath.data());
			const auto& materialDef = Materials->emplace_back(MGF::Factories::MaterialFactory::CreateMaterialDefinition(*materialFile));
			const auto material = MGF::Factories::MaterialFactory::Create(materialDef, *materialFile);

			mesh->getSubMesh(0)->setMaterial(material);

			const auto technique = material->getTechnique(0);
			const auto pass = technique->getPass(0);
			if (pass->getTextureUnitStates().size() > 0)
			{
				pass->getTextureUnitState(0)->setTextureScale(meshDef.texCoordScale, meshDef.texCoordScale);
				pass->getTextureUnitState(0)->setTextureScroll(meshDef.texCoordOffset, meshDef.texCoordOffset);
			}

			const auto entity = node->sceneNode->getCreator()->createEntity(mesh);
			node->sceneNode->attachObject(entity);

			
		}

		for (int i = 0, num_children = std::stoi(vars["num_children"].data()); i < num_children; i++)
		{
			const auto childNode = CreateSceneNode(node, func);
			if (childNode)
			{
				childNode->childIndex = node->children.size();
				node->children.push_back(childNode);
			}
		}

		return node;
	}
}

Model::Node* ModelAsset::CreateSceneNode(Model::Node* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr>& meshes)
{
	// only process nodes that include "node" in the name
	if (std::string_view type = xmlnode.name(); type.find("node_") == std::string::npos)
	{
		return nullptr;
	}

	auto node = new Model::Node;
	node->name = xmlnode.attribute("name").as_string();
	node->type = xmlnode.name();
	node->parent = parent;
	node->sceneNode = parent ? parent->sceneNode->createChildSceneNode() : SceneManager.createSceneNode();
	node->sceneNode->setPosition(StrToVector(xmlnode.attribute("position").as_string()));
	node->sceneNode->setOrientation(StrToQuat(xmlnode.attribute("rot_axis").as_string(), xmlnode.attribute("rot_angle").as_string()));
	node->sceneNode->setScale(StrToVector(xmlnode.attribute("scale").as_string()));

	// is this a node_3dobject?
	for (const auto& mesh : xmlnode.children())
	{
		// ignoring shadow meshes for now
		if (std::strcmp(mesh.name(), "mesh") == 0)
		{
			std::string meshName(mesh.attribute("name").as_string());
			auto meshPtr = meshes.at(meshName);

			Ogre::Entity* ent = node->sceneNode->getCreator()->createEntity(meshPtr);
			node->sceneNode->attachObject(ent);
		}
	}

	for (const auto& child : xmlnode.child("children").children())
	{
		auto childNode = CreateSceneNode(node, child, meshes);
		if (childNode)
		{
			childNode->childIndex = node->children.size();
			node->children.push_back(childNode);
		}
	}

	return node;
}

Ogre::Vector3 StrToVector(const std::string_view str)
{
	auto c1 = str.find(',', 0);
	auto c2 = str.find(',', c1 + 1);

	return {
		std::stof(str.substr(0, c1).data()),
		std::stof(str.substr(c1 + 1, c2).data()),
		std::stof(str.substr(c2 + 1, str.size()).data())
	};
}

Ogre::Quaternion StrToQuat(const std::string_view axis, const std::string_view angle)
{
	return { Ogre::Radian(std::stof(angle.data())), StrToVector(axis) };
}