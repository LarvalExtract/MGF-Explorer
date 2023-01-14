#include "ModelAsset.h"

#include "MGFExplorerApplication.h"
#include "MGF/AssetManager.h"
#include "MGF/Deserializer.h"

using namespace MGF::Asset;

ModelAsset::ModelAsset(const MGF::File& file) :
	AssetBase(file, MGF::Asset::EAssetType::Model),
	Nodes{std::make_shared<ModelViewer::Models::NodeTree>()},
	Animations{std::make_shared<ModelViewer::Models::AnimationTableModel>()},
	Meshes{std::make_shared<ModelViewer::Models::MeshTable>()},
	Materials{std::make_shared<ModelViewer::Models::MaterialTable>()}
{
	if (file.FileType == MGF::EFileType::mgmodel)
	{
		ParseMgmodelXml();
	}
	else if (file.FileType == MGF::EFileType::node)
	{
		ParseNodeTxt();
	}
}

void ModelAsset::ParseMgmodelXml()
{
	const MGF::Deserializer mgmodelDeserializer(FileRef);
	const auto buffer = mgmodelDeserializer.ReadAllBytes();

	pugi::xml_document document;
	document.load_string(buffer.data());
	const auto& root = document.root().first_child();
	auto node = root.first_child();

	std::unordered_map<std::string, void*> materials;
	std::unordered_map<std::string, void*> meshes;

	std::string_view name(node.name());

	// Load materials from XML
	for (; name.contains("mat"); node = node.next_sibling(), name = node.name())
	{
		if (materials.contains(node.attribute("name").as_string()))
		{
			
		}
	}

	// Load meshes from XML
	for (; name.contains("mesh"); node = node.next_sibling(), name = node.name())
	{
		if (meshes.contains(node.attribute("name").as_string()))
		{
			
		}
	}

	Nodes->RootNode = new Model::Node;
	for (; name.contains("node"); node = node.next_sibling(), name = node.name())
	{
		Nodes->RootNode->children.push_back(CreateSceneNode(Nodes->RootNode, node, meshes));
	}
}

void ModelAsset::ParseNodeTxt()
{
	const MGF::Deserializer deserializer(FileRef);
	const auto bytes = deserializer.ReadAllBytes();

	std::string_view buf_view(bytes.data());

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

Model::Node* ModelAsset::CreateSceneNode(Model::Node* parent, const std::function<ConfigSection()>& func)
{
	auto vars = func();

	if (const auto& nodeType = vars["type"]; nodeType == "ANIMNODE")
	{
		auto& rm = qApp->AssetManager;
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
		/*
		node->sceneNode = parent ? parent->sceneNode->createChildSceneNode() : SceneManager.createSceneNode();
		node->sceneNode->setPosition(StrToVector(vars["position"]));
		node->sceneNode->setOrientation(StrToQuat(vars["rot_axis"], vars["rot_angle"]));
		node->sceneNode->setScale(StrToVector(vars["scale"]));
		*/
		if (nodeType == "3DOBJECT" || nodeType == "SKIN")
		{
			const auto meshFile = FileRef.FindRelativeItem(vars["mesh"].data());

			
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

Model::Node* ModelAsset::CreateSceneNode(Model::Node* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, void*>& meshes)
{
	// only process nodes that include "node" in the name
	const std::string_view nodeType = xmlnode.name();
	if (!nodeType.contains("node_"))
	{
		return nullptr;
	}

	Model::Node* node = new Model::Node;

	std::string name = std::format("{0}.{1}", xmlnode.attribute("name").as_string(), xmlnode.name());
	node->name = xmlnode.attribute("name").as_string();
	node->type = xmlnode.name();
	node->parent = parent;
	/*
	node->sceneNode = parent ? parent->sceneNode->createChildSceneNode() : SceneManager.createSceneNode();
	node->sceneNode->setPosition(StrToVector(xmlnode.attribute("position").as_string()));
	node->sceneNode->setOrientation(StrToQuat(xmlnode.attribute("rot_axis").as_string(), xmlnode.attribute("rot_angle").as_string()));
	node->sceneNode->setScale(StrToVector(xmlnode.attribute("scale").as_string()));
	*/

	// is this a node_3dobject?
	if (nodeType.contains("3dobject"))
	{
		for (const auto& mesh : xmlnode.children("mesh"))
		{
			auto meshPtr = meshes.at(mesh.attribute("name").as_string());
			
		}
	}
	else if (nodeType.contains("skinned"))
	{
		
	}
	else if (nodeType.contains("light"))
	{
		
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
/*
Ogre::Vector3 StrToVector(const std::string_view str)
{
	const auto c1 = str.find(',', 0);
	const auto c2 = str.find(',', c1 + 1);

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
*/