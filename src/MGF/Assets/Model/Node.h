#pragma once

#include <string>
#include <OgreVector3.h>

namespace MGF {	namespace Asset { namespace Model {

	struct Node
	{
		std::string name;
		std::string type;
		Ogre::Vector3 position;

	};

} } }