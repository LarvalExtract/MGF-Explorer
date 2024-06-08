#include "ModelAsset.h"

#include "MGFExplorerApplication.h"
#include "MGF/AssetManager.h"
#include "MGF/Deserializer.h"

#include "Render/MaterialParamReader.h"
#include "Render/MaterialLibrary.h"

#include <QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QGeometryRenderer>
#include <QMaterial>
#include <QQuaternion>
#include <QVector3D>

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

	std::unordered_map<std::string, Qt3DRender::QMaterial*> materials;
	std::unordered_map<std::string, Qt3DRender::QGeometryRenderer*> meshes;

	std::string_view name(node.name());

	// Load materials from XML
	for (; name.starts_with("mat"); node = node.next_sibling(), name = node.name())
	{
		if (const std::string materialName = node.attribute("name").as_string(); !materials.contains(materialName))
		{
			materials.insert(std::make_pair(materialName, MGF::Render::MaterialLibrary::Get().GetMaterial(node, this->FileRef)));
		}
	}

	// Load meshes from XML
	for (; name.starts_with("mesh"); node = node.next_sibling(), name = node.name())
	{
		if (const std::string meshName = node.attribute("name").as_string(); !meshes.contains(meshName))
		{
			meshes.insert(std::make_pair(meshName, qApp->mMeshLibrary.CreateMesh(node, this->FileRef)));
		}
	}

	for (; name.starts_with("node"); node = node.next_sibling(), name = node.name())
	{
		CreateSceneNode(mRootNode = new Qt3DCore::QEntity, node, meshes, materials);
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

	CreateSceneNode(nullptr, ParseNodeSection);
}

void ModelAsset::CreateSceneNode(Qt3DCore::QEntity* parent, const std::function<ConfigSection()>& func)
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
	}
	else
	{
		Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(parent);
		entity->setProperty("name", vars.Name().c_str());

		auto transform = new Qt3DCore::QTransform;
		transform->setTranslation(MGF::Render::IMaterialParamReader::StrToVector(vars["position"]));
		transform->setScale3D(MGF::Render::IMaterialParamReader::StrToVector(vars["scale"]));
		transform->setRotation(MGF::Render::IMaterialParamReader::StringToQuat(vars["rot_axis"], vars["rot_angle"]));
		entity->addComponent(transform);
		
		if (nodeType == "3DOBJECT" || nodeType == "SKIN")
		{
			const MGF::File& meshFile = *FileRef.FindRelativeItem(vars["mesh"]);
			ConfigFile meshCfg(&meshFile);
			const MGF::File& materialFile = *meshFile.FindRelativeItem(meshCfg["mesh"]["material"]);

			Qt3DRender::QGeometryRenderer* geom = qApp->mMeshLibrary.CreateMesh(meshFile, this->FileRef);
			Qt3DRender::QMaterial* material = MGF::Render::MaterialLibrary::Get().GetMaterial(materialFile, this->FileRef);

			entity->addComponent(geom);
			entity->addComponent(material);
		}

		for (int i = 0, num_children = std::stoi(vars["num_children"].data()); i < num_children; i++)
		{
			CreateSceneNode(entity, func);
		}
	}
}

void ModelAsset::CreateSceneNode(Qt3DCore::QEntity* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Qt3DRender::QGeometryRenderer*>& meshes, const std::unordered_map<std::string, Qt3DRender::QMaterial*>& materials)
{
	// only process nodes that include "node" in the name
	const std::string_view nodeType = xmlnode.name();
	if (!nodeType.starts_with("node_"))
	{
		return;
	}

	Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(parent);
	entity->setProperty("name", xmlnode.attribute("name").as_string());

	auto transform = new Qt3DCore::QTransform;
	transform->setTranslation(MGF::Render::IMaterialParamReader::StrToVector(xmlnode.attribute("position").as_string()));
	transform->setScale3D(MGF::Render::IMaterialParamReader::StrToVector(xmlnode.attribute("scale").as_string()));
	transform->setRotation(MGF::Render::IMaterialParamReader::StringToQuat(xmlnode.attribute("rot_axis").as_string(), xmlnode.attribute("rot_angle").as_string()));
	entity->addComponent(transform);

	if (nodeType.contains("3dobject"))
	{
		for (const auto& mesh : xmlnode.children("mesh"))
		{
			Qt3DRender::QGeometryRenderer* geom = meshes.at(mesh.attribute("name").as_string());
			Qt3DRender::QMaterial* material = materials.at(geom->property("material").toString().toStdString());

			entity->addComponent(geom);
			entity->addComponent(material);
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
		CreateSceneNode(entity, child, meshes, materials);
	}
}


