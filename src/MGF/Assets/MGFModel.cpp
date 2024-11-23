#include "MGFModel.h"
#include "MGFExplorerApplication.h"
#include "MGF/MGFFile.h"
#include "MGF/MGFArchive.h"
#include "MGF/Deserializer.h"
#include "Widgets/ModelViewer/ModelViewerWidget.h"
#include "Render/MaterialParamReader.h"
#include "Render/MaterialLibrary.h"

#include <QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QGeometryRenderer>
#include <QMaterial>
#include <QQuaternion>
#include <QVector3D>
#include <QLayer>

MGFModel::MGFModel(const MGFFile& file) :
	MGFAsset(file)
{
	if (FileRef.FilePath().extension() == ".mgmodel")
	{
		ParseMgmodelXml();
	}
	else if (FileRef.FilePath().extension() == ".node")
	{
		ParseNodeTxt();
	}
}

void MGFModel::ParseMgmodelXml()
{
	const MGFFileDeserializer mgmodelDeserializer(FileRef);
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

	Nodes.mRootNode = mRootNode;
}

void MGFModel::ParseNodeTxt()
{
	const MGFFileDeserializer deserializer(FileRef);
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

void MGFModel::CreateSceneNode(Qt3DCore::QEntity* parent, const std::function<ConfigSection()>& func)
{
	auto vars = func();

	if (const auto& nodeType = vars["type"]; nodeType == "ANIMNODE")
	{
		auto childNodeAsset = FileRef.MgfArchive.LoadAsset<MGFModel>(vars["child"].c_str());

		int num_animations = std::stoi(vars["num_animations"]);
		for (int i = 0; i < num_animations; i++)
		{
			auto anim = func();

			//Model::Animation animDef;
			//animDef.name = std::move(anim["name"]);
			//animDef.animation = std::move(anim["animation"]);
			//animDef.synch_point = std::stof(anim["synch_point"]);
			//animDef.loop_count = std::stoi(anim["loop_count"]);
			////animDef.blend_out_duration = std::stof(anim["blend_out_duration"]);
			//animDef.primary = (anim["primary"][0] == 't');
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
			const MGFFile& meshFile = *FileRef.FindRelativeItem(vars["mesh"]);
			ConfigFile meshCfg(&meshFile);
			const MGFFile& materialFile = *meshFile.FindRelativeItem(meshCfg["mesh"]["material"]);
	
			Qt3DRender::QGeometryRenderer* geom = qApp->mMeshLibrary.CreateMesh(meshFile, this->FileRef);
			Qt3DRender::QMaterial* material = MGF::Render::MaterialLibrary::Get().GetMaterial(materialFile, this->FileRef);

			Qt3DCore::QEntity* meshEntity = new Qt3DCore::QEntity(entity);
			meshEntity->addComponent(geom);
			meshEntity->addComponent(material);
		}

		for (int i = 0, num_children = std::stoi(vars["num_children"].data()); i < num_children; i++)
		{
			CreateSceneNode(entity, func);
		}
	}
}

void MGFModel::CreateSceneNode(Qt3DCore::QEntity* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Qt3DRender::QGeometryRenderer*>& meshes, const std::unordered_map<std::string, Qt3DRender::QMaterial*>& materials)
{
	// only process nodes that include "node" in the name
	const std::string_view nodeType = xmlnode.name();
	if (!nodeType.starts_with("node_"))
	{
		return;
	}

	Qt3DCore::QEntity* entity = new Qt3DCore::QEntity(parent);

	const QString name = xmlnode.attribute("name").as_string();
	entity->setProperty("name", xmlnode.attribute("name").as_string());
	entity->setProperty("type", nodeType.data());

	Qt3DCore::QTransform* transform = new Qt3DCore::QTransform;
	const QVector3D position = MGF::Render::IMaterialParamReader::StrToVector(xmlnode.attribute("position").as_string());
	const QVector3D rotationAxis = MGF::Render::IMaterialParamReader::StrToVector(xmlnode.attribute("rot_axis").as_string());
	const float rotationAngle = qRadiansToDegrees(xmlnode.attribute("rot_angle").as_float());
	const QVector3D scale = MGF::Render::IMaterialParamReader::StrToVector(xmlnode.attribute("scale").as_string());
	const QQuaternion rotationQuat = QQuaternion::fromAxisAndAngle(rotationAxis, rotationAngle);

	transform->setTranslation(position);
	transform->setScale3D(scale);
	transform->setRotation(rotationQuat);

	entity->addComponent(transform);

	if (nodeType.contains("3dobject"))
	{
		for (const auto& mesh : xmlnode.children("mesh"))
		{
			const std::string meshName = mesh.attribute("name").as_string();
			Qt3DCore::QEntity* meshEntity = new Qt3DCore::QEntity(entity);
			meshEntity->setProperty("name", meshName.c_str());

			Qt3DRender::QGeometryRenderer* geom = meshes.at(meshName);
			Qt3DRender::QMaterial* material = materials.at(geom->property("material").toString().toStdString());

			meshEntity->addComponent(geom);
			meshEntity->addComponent(material);
			meshEntity->addComponent(material->property("blending").toString() == "normal" 
				? qApp->SceneWidget->GetTransparentLayer()
				: qApp->SceneWidget->GetOpaqueLayer()
			);
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


