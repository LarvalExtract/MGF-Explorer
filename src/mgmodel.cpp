#include "mgmodel.h"
#include "mgf/mgfarchive.h"
#include "mgf/mgmaterialfactory.h"

#include <string_view>

#include <Ogre.h>

MGModel::MGModel(MGFTreeItem& mgmodel, Ogre::SceneManager* sceneManager) :
    m_SceneMgrRef(sceneManager)
{
    LoadMGModel(mgmodel);
}

void MGModel::LoadItem(MGFTreeItem &item)
{
    // step 1: is this a NODE or MGMODEL?

}

void MGModel::LoadMGModel(MGFTreeItem &mgmodel)
{

}

void MGModel::LoadMaterial(const pugi::xml_node &matNode, MGFTreeItem& file)
{
    std::string name = matNode.attribute("name").as_string();

    if (m_Materials.find(name) == m_Materials.end())
    {
        m_Materials.emplace(name, MGMaterialFactory::Create(matNode, file));
    }

    MGMaterialInfo info;
    info.name = name.c_str();
    info.type = matNode.name();
    m_MaterialTable.push_back(info);
}

void MGModel::LoadMesh(const pugi::xml_node &meshNode, MGFTreeItem& file)
{  
    const char* name = meshNode.attribute("name").as_string();
    const char* type = meshNode.attribute("type").as_string();
    const char* material = meshNode.attribute("material").as_string();
    const char* vertices = meshNode.attribute("vertices").as_string();
    const char* indices = meshNode.attribute("indices").as_string(); 

    Ogre::String n(file.Name().toLatin1());
    n += "::";
    n += name;
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(n, "General");
    auto submesh = mesh->createSubMesh();
    submesh->setMaterial(m_Materials.at(material));

    if (std::strcmp(type, "indexedstrip") == 0)
        submesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
    else if (std::strcmp(type, "indexedlist") == 0)
        submesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    else
        throw type;
    
    uint32_t count = 0;
    uint32_t flags = 0;
    uint32_t size = 0;

    auto LoadVertexBuffer = [&](MGFTreeItem& vertFile) -> Ogre::VertexData*
    {
        Ogre::VertexData* data = new Ogre::VertexData;

        std::string buf;
        vertFile.LoadBuffer(buf);

        constexpr int vertex_count_offset = 84;
        constexpr int vertex_flags_offset = 88;
        constexpr int offset_aabb_bounds = 92;
        constexpr int vertex_buffer_size = 120;
        constexpr int vertex_data_offset = 124;

        count = *reinterpret_cast<uint32_t*>(&buf[vertex_count_offset]);
        flags = *reinterpret_cast<uint32_t*>(&buf[vertex_flags_offset]);

        std::array<float, 6> AABB_values;
        std::memcpy(&AABB_values, &buf[offset_aabb_bounds], AABB_values.size() * sizeof(float));

        size = *reinterpret_cast<uint32_t*>(&buf[vertex_buffer_size]) - 8;

        data->vertexCount = count;

        bool scaleTexCoords;// = SetupVertexElements(data->vertexDeclaration, flags);

        if (auto tu = submesh->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitStates(); tu.size() > 0)
        {
            if (scaleTexCoords)
                tu[0]->setTextureScale(1.0f / 32.0f, 1.0f / 32.0f);
        }

        auto vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    size / count,
                    count,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY,
                    false);

        vbuf->writeData(0, vbuf->getSizeInBytes(), &buf[vertex_data_offset]);

        data->vertexBufferBinding->setBinding(0, vbuf);

        mesh->_setBounds(Ogre::AxisAlignedBox(
                AABB_values[0], AABB_values[1], AABB_values[2],
                AABB_values[3], AABB_values[4], AABB_values[5]));

        return data;
    };
    auto LoadIndexBuffer = [&](MGFTreeItem& indicesFile) -> Ogre::IndexData*
    {
        Ogre::IndexData* data = new Ogre::IndexData;

        std::string buf;
        indicesFile.LoadBuffer(buf);

        constexpr int offset_index_count = 88;
        constexpr int offset_buffer_size = 96;
        constexpr int offset_index_data = 100;

        data->indexCount = *reinterpret_cast<uint32_t*>(&buf[offset_index_count]);
        data->indexStart = 0;
        data->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                    Ogre::HardwareIndexBuffer::IndexType::IT_16BIT,
                    data->indexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        data->indexBuffer->writeData(0, data->indexBuffer->getSizeInBytes(), &buf[offset_index_data]);

        return data;
    };

    QString verticesPath = file.Name() + '{' + vertices + '}';
    QString indicesPath = file.Name() + '{' + indices + '}';

    auto verticesFile = file.GetNamedSibling(verticesPath);
    auto indicesFile = file.GetNamedSibling(indicesPath);  

    submesh->useSharedVertices = false;
    submesh->vertexData = LoadVertexBuffer(const_cast<MGFTreeItem&>(*verticesFile));
    submesh->indexData = LoadIndexBuffer(const_cast<MGFTreeItem&>(*indicesFile));

    auto mat = m_Materials.at(material);
    submesh->setMaterial(mat);

    mesh->load();

    m_Meshes.emplace(name, mesh);

    MGMeshInfo info;
    info.name = name;
    info.material = material;
    info.flags = flags;
    info.numVerts = count;
    info.stride = size / count;
    info.topology = type;

    m_MeshTable.push_back(info);
}



void MGModel::BuildScene(const pugi::xml_node &node, Ogre::SceneNode* &parent)
{
    auto attr_to_vec3 = [](const pugi::xml_attribute& attr) -> Ogre::Vector3
    {
        std::string_view str(attr.as_string());

        auto c1 = str.find(',', 0);
        auto c2 = str.find(',', c1 + 1);

        float x = std::stof(str.substr(0, c1).data());
        float y = std::stof(str.substr(c1 + 1, c2).data());
        float z = std::stof(str.substr(c2 + 1, str.size()).data());

        return {x, y, z};
    };
    auto attr_to_quat = [&attr_to_vec3](const pugi::xml_attribute& rot_axis, const pugi::xml_attribute& rot_angle) -> Ogre::Quaternion
    {
        Ogre::Vector3 axis = attr_to_vec3(rot_axis);
        Ogre::Radian radians(std::stof(rot_angle.as_string()));

        return Ogre::Quaternion(radians, axis);
    };

    std::string_view nodeType(node.name());

    // only process nodes that include "node" in the name
    if (nodeType.find("node_") == std::string::npos)
        return;

    Ogre::SceneNode* sceneNode = m_SceneMgrRef->createSceneNode();
    sceneNode->setPosition(attr_to_vec3(node.attribute("position")));
    sceneNode->setOrientation(attr_to_quat(node.attribute("rot_axis"), node.attribute("rot_angle")));
    sceneNode->setScale(attr_to_vec3(node.attribute("scale")));

    if (parent)
        parent->addChild(sceneNode);
    else
        parent = sceneNode;

    // is this a node_3dobject?
    if (nodeType.find("3d", 5) != std::string::npos)
    {
        for (const auto& mesh : node.children())
        {
            // ignoring shadow meshes for now
            if (std::strcmp(mesh.name(), "mesh") == 0)
            {
                std::string meshName(mesh.attribute("name").as_string());
                auto meshPtr = m_Meshes.at(meshName);

                Ogre::Entity* ent = m_SceneMgrRef->createEntity(meshPtr);
                sceneNode->attachObject(ent);
            }
        }
    }

    for (const auto& child : node.child("children").children())
    {
        BuildScene(child, sceneNode);
    }
}

