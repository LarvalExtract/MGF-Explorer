#pragma once

#include <string>
#include <OgreRenderOperation.h>

namespace MGF {	namespace Asset { namespace Model {

	struct Mesh
	{
		std::string name;
		Ogre::RenderOperation::OperationType topology;
		std::string verticesFilename;
		std::string indicesFilename;
		std::string materialPath;
		bool bUsesMGModel = false;
		std::uint32_t numVerts = 0;
		std::uint32_t flags = 0;
		std::uint32_t stride = 0;
		float texCoordScale = 1.0f;
		float texCoordOffset = 0.0f;
	};

} } }