#pragma once

#include "AssetBase.h"

#include <OgreSceneNode.h>

namespace MGF { namespace Asset {

	struct MeshDefinition
	{

	};

	struct MaterialDefinition
	{

	};

	class Model : public AssetBase
	{
	public:
		Model(const MGF::File& file);

	private:
		Ogre::SceneNode* SceneNode = nullptr;
		std::vector<MeshDefinition> Meshes;
		std::vector<MaterialDefinition> Materials;
	};

} }