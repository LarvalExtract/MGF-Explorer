#pragma once

#include "AssetBase.h"
#include "AssetViewers/ModelViewer/Models/NodeTreeModel.h"
#include "AssetViewers/ModelViewer/Models/MeshTableModel.h"
#include "AssetViewers/ModelViewer/Models/MaterialTableModel.h"
#include "AssetViewers/ModelViewer/Models/AnimationTableModel.h"
#include "Utilities/configfile.h"

#include <OgreSceneNode.h>

#include <pugixml-1.10/src/pugixml.hpp>

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

	private:
		std::shared_ptr<ModelViewer::Models::NodeTree> Nodes;
		std::shared_ptr<ModelViewer::Models::MeshTable> Meshes;
		std::shared_ptr<ModelViewer::Models::MaterialTable> Materials;
		std::shared_ptr<ModelViewer::Models::AnimationTableModel> Animations;

		void ParseMgmodelXml();
		void ParseNodeTxt();

		Model::Node* CreateSceneNode(Model::Node* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr>& meshes);
		Model::Node* CreateSceneNode(Model::Node* parent, const std::function<ConfigSection()>& func);

		Ogre::SceneManager& SceneManager;
	};

} }