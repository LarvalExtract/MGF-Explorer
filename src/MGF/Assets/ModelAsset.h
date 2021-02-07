#pragma once

#include "AssetBase.h"
#include "Model/Node.h"
#include "Model/Mesh.h"
#include "Model/Material.h"
#include "Model/Animation.h"
#include "utils/configfile.h"

#include <OgreSceneNode.h>

#include <pugixml-1.10/src/pugixml.hpp>

namespace MGF { namespace Asset {

	class ModelAsset : public AssetBase
	{
	public:
		ModelAsset(const MGF::File& file);

		Ogre::SceneNode* GetRootNode() const { return RootNode; }
		const auto& GetNodeDefinitions() const { return NodeDefinitions; }
		const auto& GetMeshDefinitions() const { return MeshDefinitions; }
		const auto& GetMaterialDefinitions() const { return MaterialDefinitions; }
		const auto& GetAnimationDefinitions() const { return AnimationDefinitions; }

	private:
		Ogre::SceneNode* RootNode = nullptr;
		std::vector<Model::Node> NodeDefinitions;
		std::vector<Model::Mesh> MeshDefinitions;
		std::vector<Model::Material> MaterialDefinitions;
		std::vector<Model::Animation> AnimationDefinitions;

		void ParseMgmodelXml();
		void ParseNodeTxt();

		void CreateSceneNode(Ogre::SceneNode* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr> meshes);
		void CreateSceneNode(Ogre::SceneNode* parent, const std::function<ConfigSection()>& func);
	};

} }