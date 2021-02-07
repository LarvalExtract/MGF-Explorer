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

	public:
		static Ogre::MeshPtr Create(const MGF::Asset::Model::Mesh& def, const MGF::File& sourceFile);

		static MGF::Asset::Model::Mesh CreateMeshDefinition(const MGF::File& meshFile);
		static MGF::Asset::Model::Mesh CreateMeshDefinition(const pugi::xml_node& meshxml);

	private:


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

		static Ogre::VertexData* LoadVertexBuffer(Ogre::Mesh& mesh, const MGF::File& vertFile, const MGVertexBufferOffsets& def, MGF::Asset::Model::Mesh& meshDef);
		static Ogre::IndexData* LoadIndexBuffer(const MGF::File& indicesFile, const MGIndexBufferOffsets& def);
		static bool SetupVertexElements(Ogre::VertexDeclaration* decl, uint32_t flags);
	};

}}

#endif // MGMESHFACTORY_H
