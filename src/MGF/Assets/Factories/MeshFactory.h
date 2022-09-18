#ifndef MGMESHFACTORY_H
#define MGMESHFACTORY_H

#include "MGF/Assets/ModelAsset.h"

#include <OgreMesh.h>
#include <OgreRenderOperation.h>

#include <pugixml-1.10/src/pugixml.hpp>

namespace MGF { namespace Factories { 

	class MeshFactory
	{
	public:
		MeshFactory() = delete;
		MeshFactory(const MeshFactory&) = delete;
		MeshFactory(MeshFactory&&) = delete;
		MeshFactory& operator=(const MeshFactory&) = delete;
		MeshFactory& operator=(MeshFactory&&) = delete;

		static Ogre::MeshPtr Create(const pugi::xml_node& meshXml, const MGF::File& mgmodelFile, Asset::Model::Mesh& meshDef);
		static Ogre::MeshPtr Create(const MGF::File& meshFile, Asset::Model::Mesh& meshDef);

	private:
		static MGF::Asset::Model::Mesh CreateMeshDefinition(const MGF::File& meshFile);
		static MGF::Asset::Model::Mesh CreateMeshDefinition(const pugi::xml_node& meshxml);

		struct MGVertexBufferOffsets
		{
			uint32_t count = 0;
			uint32_t flags = 0;
			uint32_t aabb_bounds = 0;
			uint32_t size = 0;
			uint32_t data = 0;
		};

		struct MGIndexBufferOffsets
		{
			uint32_t count = 0;
			uint32_t size = 0;
			uint32_t data = 0;
		};

		static bool SetupVertexElements(Ogre::VertexDeclaration* decl, uint32_t flags);
	};

}}

#endif // MGMESHFACTORY_H
