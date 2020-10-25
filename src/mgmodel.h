#ifndef MGMODELFACTORY_H
#define MGMODELFACTORY_H

#include "mgf/mgftreeitem.h"

#include <unordered_map>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>

#include <pugixml.hpp>

struct MGMeshInfo
{
    QString name;
    QString material;
    uint32_t numVerts;
    uint32_t flags;
    uint32_t stride;
    QString topology;
};

struct MGMaterialInfo
{
    QString name;
    QString type;
};

class MGModel
{
public:
    friend class MeshTableModel;
    friend class MaterialTableModel;

    MGModel(MGFTreeItem& mgmodel, Ogre::SceneManager* sceneManager);

    void LoadItem(MGFTreeItem& item);
    void LoadMGModel(MGFTreeItem& mgmodel);

    Ogre::SceneNode* GetNode() { return m_RootNode; }

private:
    void LoadMaterial(const pugi::xml_node& matNode, MGFTreeItem& file);
    void LoadMesh(const pugi::xml_node& meshNode, MGFTreeItem& file);

    // returns true if the texture co-ordinates SHOULD be scaled
    //bool SetupVertexElements(Ogre::VertexDeclaration* decl, uint32_t flags);

    void BuildScene(const pugi::xml_node& node, Ogre::SceneNode* &parent);

private:
    std::unordered_map<std::string, Ogre::MeshPtr> m_Meshes;
    std::unordered_map<std::string, Ogre::MaterialPtr> m_Materials;
    Ogre::SceneNode* m_RootNode = nullptr;
    Ogre::SceneManager* m_SceneMgrRef;

    std::vector<MGMeshInfo> m_MeshTable;
    std::vector<MGMaterialInfo> m_MaterialTable;
};

#endif // MGMODELFACTORY_H
