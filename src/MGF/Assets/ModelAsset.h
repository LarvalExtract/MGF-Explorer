#pragma once

#include "AssetBase.h"
#include "Model/Node.h"
#include "Model/Mesh.h"
#include "Model/Material.h"
#include "Model/Animation.h"
#include "Utilities/configfile.h"

#include <OgreSceneNode.h>

#include <pugixml-1.10/src/pugixml.hpp>

namespace MGF { namespace Asset {

	class ModelAsset : public AssetBase
	{
	public:
		ModelAsset(const MGF::File& file);

		Ogre::SceneNode* GetRootNode() const { return RootNode.sceneNode; }
		const auto& GetNodeDefinitions() const { return NodeDefinitions; }
		const auto& GetMeshDefinitions() const { return MeshDefinitions; }
		const auto& GetMaterialDefinitions() const { return MaterialDefinitions; }
		const auto& GetAnimationDefinitions() const { return AnimationDefinitions; }

	private:
		Model::Node RootNode;
		std::vector<Model::Node> NodeDefinitions;
		std::vector<Model::Mesh> MeshDefinitions;
		std::vector<Model::Material> MaterialDefinitions;
		std::vector<Model::Animation> AnimationDefinitions;

		void ParseMgmodelXml();
		void ParseNodeTxt();

		void CreateSceneNode(Model::Node* parent, int parentIndex, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr>& meshes);
		void CreateSceneNode(Ogre::SceneNode* parent, const std::function<ConfigSection()>& func);
	};

} }