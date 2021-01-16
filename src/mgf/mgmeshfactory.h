#ifndef MGMESHFACTORY_H
#define MGMESHFACTORY_H

#include "File.h"

#include <OgreMesh.h>
#include <OgreRenderOperation.h>
#include <pugixml-1.10/src/pugixml.hpp>

struct MGMeshDef
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

class MGMeshFactory
{
public:
    MGMeshFactory() = delete;
    MGMeshFactory(const MGMeshFactory&) = delete;
    MGMeshFactory(MGMeshFactory&&) = delete;
    MGMeshFactory& operator=(const MGMeshFactory&) = delete;
    MGMeshFactory& operator=(MGMeshFactory&&) = delete;

public:
    static Ogre::MeshPtr Create(const MGMeshDef& def, const MGF::File& sourceFile);

    static MGMeshDef CreateMeshDefinition(const MGF::File& meshFile);
    static MGMeshDef CreateMeshDefinition(const pugi::xml_node& meshxml);

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

    static Ogre::VertexData* LoadVertexBuffer(Ogre::Mesh& mesh, const MGF::File& vertFile, const MGVertexBufferOffsets& def, MGMeshDef& meshDef);
    static Ogre::IndexData* LoadIndexBuffer(const MGF::File& indicesFile, const MGIndexBufferOffsets& def);
    static bool SetupVertexElements(Ogre::VertexDeclaration *decl, uint32_t flags);
};

#endif // MGMESHFACTORY_H
