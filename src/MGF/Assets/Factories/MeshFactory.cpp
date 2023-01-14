#include "MeshFactory.h"
#include "MaterialFactory.h"
#include "Utilities/configfile.h"

#include "MGF/Deserializer.h"
#include "MGF/Structures/geomFace.h"
#include "MGF/Structures/geomVert.h"

/*
static std::unordered_map<std::string_view, Ogre::RenderOperation::OperationType> MapTopologies = {
    { "indexedlist",    Ogre::RenderOperation::OT_TRIANGLE_LIST },
    { "indexedstrip",   Ogre::RenderOperation::OT_TRIANGLE_STRIP },
    { "INDEXEDLIST",    Ogre::RenderOperation::OT_TRIANGLE_LIST },
    { "INDEXEDSTRIP",   Ogre::RenderOperation::OT_TRIANGLE_STRIP}
};

using namespace MGF::Factories;

Ogre::MeshPtr MeshFactory::Create(const pugi::xml_node& meshXml, const MGF::File& mgmodelFile, Asset::Model::Mesh& meshDef)
{
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName(meshXml.name());
    if (mesh)
    {
        return mesh;
    }

	mesh = Ogre::MeshManager::getSingleton().create(meshXml.name(), "General");

    MGF::Deserializer vbDeserializer(*mgmodelFile.FindRelativeItem(meshXml.attribute("vertices").as_string()));
    MGF::Deserializer ibDeserializer(*mgmodelFile.FindRelativeItem(meshXml.attribute("indices").as_string()));

    const auto vbHeader = vbDeserializer.Deserialize<GEOMVERT_MA2>();
    const auto ibHeader = ibDeserializer.Deserialize<GEOMFACE_MA2>();

    std::vector<char> vertices(vbHeader.vers.data_size - 8);
    std::vector<uint16_t> indices(ibHeader.indx.nidx.indexCount);

    vbDeserializer.ReadBytes(vertices.data(), vertices.size());
    ibDeserializer.ReadBytes(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(uint16_t));

    const auto vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        (vbHeader.vers.data_size - 8) / vbHeader.vers.info.vertexCount,
        vbHeader.vers.info.vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY,
        false
    );

    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices.data());

    const auto vertexData = new Ogre::VertexData;
    vertexData->vertexBufferBinding->setBinding(0, vbuf);
    if (SetupVertexElements(vertexData->vertexDeclaration, vbHeader.vers.info.vertexBufferLayoutFlags))
    {
        // meshDef.texCoordScale = 1.0f / 32.0f;
        // meshDef.texCoordOffset = 0.5f;
    }

    const auto indexData = new Ogre::IndexData;
    indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_16BIT, 
        ibHeader.indx.nidx.indexCount, 
        Ogre::HardwareBuffer::HBU_WRITE_ONLY
    );
    indexData->indexStart = 0;
    indexData->indexCount = ibHeader.indx.nidx.indexCount;
    indexData->indexBuffer->writeData(0, indexData->indexBuffer->getSizeInBytes(), indices.data());

    const auto submesh = mesh->createSubMesh();
    submesh->vertexData = vertexData;
    submesh->indexData = indexData;
    submesh->useSharedVertices = false;
    submesh->operationType = MapTopologies.at(meshXml.attribute("type").as_string());

    const auto [min, max] = vbHeader.vers.info.extents;
    mesh->_setBounds({ min.x, min.y, min.z, max.x, max.y, max.z });
    mesh->load();

    meshDef = CreateMeshDefinition(meshXml);

    return mesh;
}

Ogre::MeshPtr MeshFactory::Create(const MGF::File& meshFile, Asset::Model::Mesh& meshDef)
{
    ConfigFile meshCfg(&meshFile);
    const auto& vars = meshCfg["mesh"];

    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName(vars.at("name"));
    if (mesh)
    {
        return mesh;
    }

    mesh = Ogre::MeshManager::getSingleton().create(vars.at("name"), "General");

    MGF::Deserializer vbDeserializer(*meshFile.FindRelativeItem(vars.at("vertices")));
    MGF::Deserializer ibDeserializer(*meshFile.FindRelativeItem(vars.at("faces")));

    const auto vbHeader = vbDeserializer.Deserialize<GEOMVERT_MA1>();
    const auto ibHeader = ibDeserializer.Deserialize<GEOMFACE_MA1>();

    std::vector<char> vertices(vbHeader.vers.data_size - 8);
    std::vector<uint16_t> indices(ibHeader.indx.nidx.indexCount);

    vbDeserializer.ReadBytes(vertices.data(), vertices.size());
    ibDeserializer.ReadBytes(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(uint16_t));

    const auto vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        (vbHeader.vers.data_size - 8) / vbHeader.vers.info.vertexCount,
        vbHeader.vers.info.vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY,
        false
    );

    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices.data());

    const auto vertexData = new Ogre::VertexData;
    vertexData->vertexBufferBinding->setBinding(0, vbuf);
    if (SetupVertexElements(vertexData->vertexDeclaration, vbHeader.vers.info.vertexBufferLayoutFlags))
    {
        // meshDef.texCoordScale = 1.0f / 32.0f;
        // meshDef.texCoordOffset = 0.5f;
    }

    const auto indexData = new Ogre::IndexData;
    indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_16BIT,
        ibHeader.indx.nidx.indexCount,
        Ogre::HardwareBuffer::HBU_WRITE_ONLY
    );
    indexData->indexStart = 0;
    indexData->indexCount = ibHeader.indx.nidx.indexCount;
    indexData->indexBuffer->writeData(0, indexData->indexBuffer->getSizeInBytes(), indices.data());

    const auto submesh = mesh->createSubMesh();
    submesh->vertexData = vertexData;
    submesh->indexData = indexData;
    submesh->useSharedVertices = false;
    submesh->operationType = MapTopologies.at(vars.at("type"));

    const auto [min, max] = vbHeader.vers.info.extents;
    mesh->_setBounds({ min.x, min.y, min.z, max.x, max.y, max.z });
    mesh->load();

    meshDef = CreateMeshDefinition(meshFile);

    return mesh;
}

MGF::Asset::Model::Mesh MeshFactory::CreateMeshDefinition(const MGF::File &meshFile)
{
    ConfigFile meshCfg(&meshFile);
    const auto& vars = meshCfg["mesh"];

    MGF::Asset::Model::Mesh meshDef;
    meshDef.name = vars.at("name");
    meshDef.topology = MapTopologies.at(vars.at("type"));
    meshDef.verticesFilename = vars.at("vertices");
    meshDef.indicesFilename = vars.at("faces");
    meshDef.materialPath = vars.at("material");
    meshDef.bUsesMGModel = false;

    return meshDef;
}

MGF::Asset::Model::Mesh MeshFactory::CreateMeshDefinition(const pugi::xml_node &meshxml)
{
    MGF::Asset::Model::Mesh meshDef;
    meshDef.name = meshxml.attribute("name").as_string();
    meshDef.topology = MapTopologies.at(meshxml.attribute("type").as_string());
    meshDef.verticesFilename = meshxml.attribute("vertices").as_string();
    meshDef.indicesFilename = meshxml.attribute("indices").as_string();
    meshDef.materialPath = meshxml.attribute("material").as_string();
    meshDef.bUsesMGModel = true;

    return meshDef;
}

bool MeshFactory::SetupVertexElements(Ogre::VertexDeclaration *decl, uint32_t flags)
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
        throw std::runtime_error("Unrecognized vertex buffer flags");
    }

    return bScaleTexCoords;
}
*/