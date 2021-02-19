#include "mgnode.h"
#include "mgfresourcemanager.h"
#include "utils/ContextProvider.h"

#include <filesystem>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

MGNode::MGNode(const MGF::File& nodeFile, Ogre::SceneManager* sceneMgr) :
    m_SourceFile(nodeFile),
    m_Root(sceneMgr->createSceneNode())
{  
    switch (nodeFile.FileType())
    {
    case MGFFileType::Node:     LoadScene(nodeFile); break;
    case MGFFileType::Model:    LoadMGModel(nodeFile); break;
    }

    m_Root->_updateBounds();
    m_NodeTreeModel.RootNode = m_Root;
}

void MGNode::LoadScene(const MGF::File &mgnodeFile)
{
    std::string buf;
    mgnodeFile.LoadBuffer(buf);
    std::string_view buf_view(buf);

    std::size_t chunkStart = buf_view.find('[', 0);
    std::size_t chunkEnd = buf_view.find('[', chunkStart+1);
    std::string_view chunk;

    auto ParseNodeSection = [&]()
    {
        chunkStart = chunkEnd;
        chunkEnd = buf_view.find('[', chunkEnd+1);
        chunk = buf_view.substr(chunkStart, chunkEnd - chunkStart);

        return ConfigSection(chunk);
    };

    CreateSceneNode(m_Root, ParseNodeSection);
}

void MGNode::LoadMGModel(const MGF::File &mgmodelFile)
{
    std::string buf;
    mgmodelFile.LoadBuffer(buf);

    pugi::xml_document doc;
    doc.load_string(buf.c_str());

    const auto& xmlres = doc.first_child();
    auto node = xmlres.first_child();

    std::string_view name(node.name());

    std::unordered_map<std::string, Ogre::MaterialPtr> materials;
    std::unordered_map<std::string, Ogre::MeshPtr> meshes;

    // Load materials from XML
    for ( ; name.find("mat") != std::string::npos; node = node.next_sibling(), name = node.name())
    {
        std::string matName(node.attribute("name").as_string());

        if (materials.find(matName) == materials.end())
        {
            auto& matDef = m_MaterialTableModel.MaterialDefinitions.emplace_back(MaterialFactory::CreateMaterialDefinition(node));

            auto material = MaterialFactory::Create(matDef, mgmodelFile);

            materials.insert(std::make_pair(matName, material));
        }
    }

    // Load meshes from XML
    for ( ; name.find("mesh") != std::string::npos; node = node.next_sibling(), name = node.name())
    {
        std::string meshName(node.attribute("name").as_string());

        if (meshes.find(meshName) == meshes.end())
        {
            auto& meshDef = m_MeshTableModel.MeshDefinitions.emplace_back(MeshFactory::CreateMeshDefinition(node));

            auto mesh = MeshFactory::Create(meshDef, m_SourceFile);

            auto material = materials.at(node.attribute("material").as_string());
            mesh->getSubMesh(0)->setMaterial(material);

            auto technique = material->getTechnique(0);
            auto pass = technique->getPass(0);
            if (pass->getTextureUnitStates().size() > 0)
            {
                pass->getTextureUnitState(0)->setTextureScale(meshDef.texCoordScale, meshDef.texCoordScale);
                pass->getTextureUnitState(0)->setTextureScroll(meshDef.texCoordOffset, meshDef.texCoordOffset);
            }

            meshes.insert(std::make_pair(meshName, mesh));
        }
    }

    CreateSceneNode(m_Root, xmlres.last_child(), meshes);
}

static uint64_t unique_id = 0;

void MGNode::CreateSceneNode(Ogre::SceneNode *parent, const std::function<ConfigSection()> &func)
{
    auto vars = func();

    std::string name(vars.Name());
    name += '@' + vars["type"];
    name += '@' + std::to_string(++unique_id);

    Ogre::SceneNode* sceneNode = parent->createChildSceneNode(name);
    sceneNode->setPosition(StrToVector(vars["position"]));
    sceneNode->setOrientation(StrToQuat(vars["rot_axis"], vars["rot_angle"]));
    sceneNode->setScale(StrToVector(vars["scale"]));
    sceneNode->getUserObjectBindings().setUserAny("isVisible", Ogre::Any(true));

    const std::string& type = vars["type"];
    if (type == "ANIMNODE")
    {
        auto& rm = *Contexts::Get<MGFResourceManager>();

        auto childNodeFile = m_SourceFile.FindRelativeItem(vars["child"].c_str());

        auto& childNode = rm.GetModel(*childNodeFile, sceneNode->getCreator());
        m_Root = childNode.SceneNode();

        // grab these from the child node
        m_MeshTableModel.MeshDefinitions = childNode.GetMeshTableModel()->MeshDefinitions;
        m_MaterialTableModel.MaterialDefinitions = childNode.GetMaterialTableModel()->MaterialDefinitions;

        int num_animations = std::stoi(vars["num_animations"]);
        for (int i = 0; i < num_animations; i++)
        {
            auto anim = func();

            MGAnimDef animDef;
            animDef.name = std::move(anim["name"]);
            animDef.animation = std::move(anim["animation"]);
            animDef.synch_point = std::stof(anim["synch_point"]);
            animDef.loop_count = std::stoi(anim["loop_count"]);
            //animDef.blend_out_duration = std::stof(anim["blend_out_duration"]);
            animDef.primary = (anim["primary"][0] == 't');

            m_AnimationTableModel.AnimationDefinitions.push_back(std::move(animDef));
        }

        // TODO: Parse animations
        return; // there won't be anymore nodes, exit the function
    }
    else if (type == "3DOBJECT" || type == "SKIN")
    {
        auto meshFile = m_SourceFile.FindRelativeItem(vars["mesh"].data());
        auto& meshDef = m_MeshTableModel.MeshDefinitions.emplace_back(MeshFactory::CreateMeshDefinition(*meshFile));
        auto mesh = MeshFactory::Create(meshDef, *meshFile);

        auto materialFile = meshFile->FindRelativeItem(meshDef.materialPath.data());
        auto& materialDef = m_MaterialTableModel.MaterialDefinitions.emplace_back(MaterialFactory::CreateMaterialDefinition(*materialFile));
        auto material = MaterialFactory::Create(materialDef, *materialFile);

        mesh->getSubMesh(0)->setMaterial(material);

        auto technique = material->getTechnique(0);
        auto pass = technique->getPass(0);
        if (pass->getTextureUnitStates().size() > 0)
        {
            pass->getTextureUnitState(0)->setTextureScale(meshDef.texCoordScale, meshDef.texCoordScale);
            pass->getTextureUnitState(0)->setTextureScroll(meshDef.texCoordOffset, meshDef.texCoordOffset);
        }

        Ogre::Entity* ent = sceneNode->getCreator()->createEntity(mesh);
        sceneNode->attachObject(ent);
    }

    int num_children = std::stoi(vars["num_children"].data());
    for (int i = 0; i < num_children; i++)
    {
        CreateSceneNode(sceneNode, func);
    }
}

void MGNode::CreateSceneNode(Ogre::SceneNode *parent, const pugi::xml_node &xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr> meshes)
{
    std::string_view nodeType(xmlnode.name());

    // only process nodes that include "node" in the name
    if (nodeType.find("node_") == std::string::npos)
        return;

    // Many models across mechassault share the same node names
    // Ogre requires unique node names for some reason, so a unique name is generated using an incrementing static int
    // The node's type is also encoded in its name so it can be fetched later
    std::string name(xmlnode.attribute("name").as_string());
    name += '@';
    name += xmlnode.name();
    name += '@' + std::to_string(++unique_id);

    Ogre::SceneNode* sceneNode = parent->createChildSceneNode(name);
    sceneNode->setPosition(StrToVector(xmlnode.attribute("position").as_string()));
    sceneNode->setOrientation(StrToQuat(xmlnode.attribute("rot_axis").as_string(), xmlnode.attribute("rot_angle").as_string()));
    sceneNode->setScale(StrToVector(xmlnode.attribute("scale").as_string()));
    sceneNode->getUserObjectBindings().setUserAny("isVisible", Ogre::Any(true)); // used to toggle visibility

    // is this a node_3dobject?
    if (nodeType.find("3d", 5) != std::string::npos ||
        nodeType.find("skin", 5) != std::string::npos)
    {
        for (const auto& mesh : xmlnode.children())
        {
            // ignoring shadow meshes for now
            if (std::strcmp(mesh.name(), "mesh") == 0)
            {
                std::string meshName(mesh.attribute("name").as_string());
                auto meshPtr = meshes.at(meshName);

                Ogre::Entity* ent = sceneNode->getCreator()->createEntity(meshPtr);
                sceneNode->attachObject(ent);
            }
        }
    }

    for (const auto& child : xmlnode.child("children").children())
    {
        CreateSceneNode(sceneNode, child, meshes);
    }
}

Ogre::Vector3 MGNode::StrToVector(const std::string_view str)
{
    auto c1 = str.find(',', 0);
    auto c2 = str.find(',', c1+1);

    return {
        std::stof(str.substr(0, c1).data()),
        std::stof(str.substr(c1+1, c2).data()),
        std::stof(str.substr(c2+1, str.size()).data())
    };
}

Ogre::Quaternion MGNode::StrToQuat(const std::string_view axis, const std::string_view angle)
{
    return { Ogre::Radian(std::stof(angle.data())), StrToVector(axis) };
}


