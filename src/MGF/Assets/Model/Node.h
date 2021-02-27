#pragma once

#include <string>
#include <vector>
#include <OgreSceneNode.h>

namespace MGF {	namespace Asset { namespace Model {

	struct Node
	{
		std::string name;
		std::string type;
		bool bVisible = true;

		Ogre::SceneNode* sceneNode = nullptr;
		std::vector<Node*> children;
		int parentIndex;
	};

} } }