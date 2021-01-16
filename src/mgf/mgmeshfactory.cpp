#include "mgmeshfactory.h"
#include "mgmaterialfactory.h"
#include "utils/configfile.h"

#include <sstream>

#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBuffer.h>
#include <OgreHardwareBufferManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>

static std::unordered_map<std::string_view, Ogre::RenderOperation::OperationType> MapTopologies = {
    { "indexedlist",    Ogre::RenderOperation::OT_TRIANGLE_LIST },
    { "indexedstrip",   Ogre::RenderOperation::OT_TRIANGLE_STRIP },
    { "INDEXEDLIST",    Ogre::RenderOperation::OT_TRIANGLE_LIST },
    { "INDEXEDSTRIP",   Ogre::RenderOperation::OT_TRIANGLE_STRIP}
};

Ogre::MeshPtr MGMeshFactory::Create(const MGMeshDef &def, const MGF::File& sourceFile)
{
    QString vbPath, ibPath;
    if (def.bUsesMGModel)
    {
        vbPath = sourceFile.Name();
        vbPath += '{';
        vbPath += def.verticesFilename.data();
        vbPath += '}';

        ibPath = sourceFile.Name();
        ibPath += '{';
        ibPath += def.indicesFilename.data();
        ibPath += '}';
    }
    else
    {
        vbPath = def.verticesFilename.data();
        ibPath = def.indicesFilename.data();
    }

    const auto& vertices = *sourceFile.FindRelativeItem(vbPath);
    const auto& indices = *sourceFile.FindRelativeItem(ibPath);

    // Generate unique mesh name using source file name + mesh name
    Ogre::String name(std::to_string(vertices.GUID()));

    // Create mesh and default submesh
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName(name, "General");

    if (!mesh)
    {
        mesh = Ogre::MeshManager::getSingleton().createManual(name, "General");

        Ogre::SubMesh* submesh = mesh->createSubMesh();

        MGVertexBufferOffsets vtxOffsets;
        MGIndexBufferOffsets idxOffsets;

        if (def.bUsesMGModel)
        {
            // MGModel style buffer offsets
            vtxOffsets.count = 84;
            vtxOffsets.flags = 88;
            vtxOffsets.aabb_bounds = 92;
            vtxOffsets.size = 120;
            vtxOffsets.data = 124;

            idxOffsets.count = 88;
            idxOffsets.size = 96;
            idxOffsets.data = 100;
        }
        else
        {
            // .mesh file style offsets
            vtxOffsets.count = 60;
            vtxOffsets.flags = 64;
            vtxOffsets.aabb_bounds = 68;
            vtxOffsets.size = 96;
            vtxOffsets.data = 100;

            idxOffsets.count = 65;
            idxOffsets.size = 73;
            idxOffsets.data = 77;
        }

        submesh->operationType = def.topology;
        submesh->useSharedVertices = false;
        submesh->vertexData = LoadVertexBuffer(*mesh, vertices, vtxOffsets, const_cast<MGMeshDef&>(def));
        submesh->indexData = LoadIndexBuffer(indices, idxOffsets);

        mesh->load();
    }

    return mesh;
}

MGMeshDef MGMeshFactory::CreateMeshDefinition(const MGF::File &meshFile)
{
    std::string buf;
    meshFile.LoadBuffer(buf);

    ConfigFile meshCfg(buf);
    auto& vars = meshCfg["mesh"];

    MGMeshDef meshDef;
    meshDef.name = vars["name"];
    meshDef.topology = MapTopologies[vars["type"]];
    meshDef.verticesFilename = vars["vertices"];
    meshDef.indicesFilename = vars["faces"];
    meshDef.materialPath = vars["material"];
    meshDef.bUsesMGModel = false;

    return meshDef;
}

MGMeshDef MGMeshFactory::CreateMeshDefinition(const pugi::xml_node &meshxml)
{
    MGMeshDef meshDef;
    meshDef.name = meshxml.attribute("name").as_string();
    meshDef.topology = MapTopologies[meshxml.attribute("type").as_string()];
    meshDef.verticesFilename = meshxml.attribute("vertices").as_string();
    meshDef.indicesFilename = meshxml.attribute("indices").as_string();
    meshDef.materialPath = meshxml.attribute("material").as_string();
    meshDef.bUsesMGModel = true;

    return meshDef;
}

Ogre::VertexData *MGMeshFactory::LoadVertexBuffer(Ogre::Mesh& mesh, const MGF::File &vertFile, const MGMeshFactory::MGVertexBufferOffsets &def, MGMeshDef& meshDef)
{
    Ogre::VertexData* data = new Ogre::VertexData;

    std::string buf;
    vertFile.LoadBuffer(buf);

    uint32_t count = *reinterpret_cast<uint32_t*>(&buf[def.count]);
    uint32_t flags = *reinterpret_cast<uint32_t*>(&buf[def.flags]);

    std::array<float, 6> AABB_values;
    std::memcpy(&AABB_values, &buf[def.aabb_bounds], AABB_values.size() * sizeof(float));

    uint32_t size = *reinterpret_cast<uint32_t*>(&buf[def.size]) - 8;

    data->vertexCount = count;

    bool bScaleTexCoords = SetupVertexElements(data->vertexDeclaration, flags);

    auto vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                size / count,
                count,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY,
                false);

    vbuf->writeData(0, vbuf->getSizeInBytes(), &buf[def.data]);

    data->vertexBufferBinding->setBinding(0, vbuf);

    mesh._setBounds(Ogre::AxisAlignedBox(
            AABB_values[0], AABB_values[1], AABB_values[2],
            AABB_values[3], AABB_values[4], AABB_values[5]));

    meshDef.numVerts = count;
    meshDef.flags = flags;
    meshDef.stride = size / count;

    if (bScaleTexCoords)
    {
        meshDef.texCoordScale = 1.0f / 32.0f;
        meshDef.texCoordOffset = 0.5f;
    }

    return data;
}

Ogre::IndexData *MGMeshFactory::LoadIndexBuffer(const MGF::File &indicesFile, const MGMeshFactory::MGIndexBufferOffsets &def)
{
    Ogre::IndexData* data = new Ogre::IndexData;

    std::string buf;
    indicesFile.LoadBuffer(buf);

    data->indexCount = *reinterpret_cast<uint32_t*>(&buf[def.count]);
    data->indexStart = 0;
    data->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                Ogre::HardwareIndexBuffer::IndexType::IT_16BIT,
                data->indexCount,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    data->indexBuffer->writeData(0, data->indexBuffer->getSizeInBytes(), &buf[def.data]);

    return data;
}

bool MGMeshFactory::SetupVertexElements(Ogre::VertexDeclaration *decl, uint32_t flags)
{
    using namespace Ogre;

    bool bScaleTexCoords = true;
    decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

    switch (flags)
    {
    case 0x00000003:    // 24 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 12, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        bScaleTexCoords = false;
        break;

    case 0x00000101:    // 16 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        break;

    case 0x00000301:    // 20 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 16, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00000309:    // 24 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 20, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00000501:    // 24 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 16, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00000509:
        decl->addElement(0, 20, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00001003:    // 32 bytes
        //decl->addElement(0, 12, VET_FLOAT3, VES_NORMAL); // no idea
        decl->addElement(0, 24, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        bScaleTexCoords = false;
        break;

    case 0x00001101:    // 24 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 16, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        bScaleTexCoords = false;
        break;

    case 0x00002003:    // 40 bytes
        //decl->addElement(0, 12, VET_FLOAT3, VES_NORMAL); // no idea
        decl->addElement(0, 24, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        bScaleTexCoords = false;
        break;

    case 0x00002101:    // 32 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        decl->addElement(0, 16, VET_FLOAT2, VES_TEXTURE_COORDINATES);
        bScaleTexCoords = false;
        break;

    case 0x00080101:    // 36 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        //decl->addElement(0, 16, VET_FLOAT4, VES_BLEND_WEIGHTS); // maybe?
        decl->addElement(0, 36, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00080301:    // 40 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        //decl->addElement(0, 16, VET_FLOAT4, VES_BLEND_WEIGHTS); // maybe?
        decl->addElement(0, 16, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00100301:    // 36 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        //decl->addElement(0, 12, VET_FLOAT4, VES_BLEND_WEIGHTS); // maybe?
        decl->addElement(0, 32, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00100501:    // 40 bytes
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        //decl->addElement(0, 16, VET_BYTE4, VES_SPECULAR);
        //decl->addElement(0, 20, VET_FLOAT3, VES_BLEND_WEIGHTS);
        decl->addElement(0, 32, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    case 0x00180301:
        //decl->addElement(0, 12, VET_UBYTE4_NORM, VES_DIFFUSE); // no idea
        //decl->addElement(0, 16, VET_FLOAT4, VES_BLEND_WEIGHTS); // maybe?
        decl->addElement(0, 32, VET_SHORT2_NORM, VES_TEXTURE_COORDINATES);
        break;

    default:
        throw "Unrecognised vertex buffer flags";
        break;
    }

    return bScaleTexCoords;
}
