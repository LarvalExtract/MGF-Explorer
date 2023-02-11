#pragma once

#include "AssetBase.h"
#include "AssetViewers/ModelViewer/Models/NodeTreeModel.h"
#include "AssetViewers/ModelViewer/Models/MeshTableModel.h"
#include "AssetViewers/ModelViewer/Models/MaterialTableModel.h"
#include "AssetViewers/ModelViewer/Models/AnimationTableModel.h"
#include "Utilities/configfile.h"

#include <pugixml-1.10/src/pugixml.hpp>

namespace Qt3DCore
{
	class QEntity;
}

namespace Qt3DRender
{
	class QGeometryRenderer;
	class QMaterial;
}

namespace MGF { namespace Asset {

	class ModelAsset : public AssetBase
	{
	public:
		ModelAsset(const MGF::File& file);

		const auto GetRootNode() const { return Nodes->RootNode; }
		const auto GetNodeTreeModel() { return Nodes.get(); }
		const auto GetMeshTableModel() { return Meshes.get(); }
		const auto GetMaterialTableModel() { return Materials.get(); }
		const auto GetAnimationTableModel() { return Animations.get(); }

		Qt3DCore::QEntity* mRootNode = nullptr;

	private:
		std::shared_ptr<ModelViewer::Models::NodeTree> Nodes;
		std::shared_ptr<ModelViewer::Models::MeshTable> Meshes;
		std::shared_ptr<ModelViewer::Models::MaterialTable> Materials;
		std::shared_ptr<ModelViewer::Models::AnimationTableModel> Animations;

		void ParseMgmodelXml();
		void ParseNodeTxt();

		void CreateSceneNode(Qt3DCore::QEntity* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Qt3DRender::QGeometryRenderer*>& meshes, const std::unordered_map<std::string, Qt3DRender::QMaterial*>& materials);
		void CreateSceneNode(Qt3DCore::QEntity* parent, const std::function<ConfigSection()>& func);
	};

} }