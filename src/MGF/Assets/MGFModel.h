#pragma once

#include "MGFAsset.h"
#include "Widgets/ModelViewer/Models/NodeTreeModel.h"
#include "Widgets/ModelViewer/Models/MeshTableModel.h"
#include "Widgets/ModelViewer/Models/MaterialTableModel.h"
#include "Widgets/ModelViewer/Models/AnimationTableModel.h"
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

class MGFModel : public MGFAsset
{
public:
	MGFModel(const MGFFile& file);

	Qt3DCore::QEntity* mRootNode = nullptr;

	ModelViewer::Models::NodeTree Nodes;
	ModelViewer::Models::MeshTable Meshes;
	ModelViewer::Models::MaterialTable Materials;
	ModelViewer::Models::AnimationTableModel Animations;

	void ParseMgmodelXml();
	void ParseNodeTxt();

	void CreateSceneNode(Qt3DCore::QEntity* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Qt3DRender::QGeometryRenderer*>& meshes, const std::unordered_map<std::string, Qt3DRender::QMaterial*>& materials);
	void CreateSceneNode(Qt3DCore::QEntity* parent, const std::function<ConfigSection()>& func);
};