#ifndef MGNODE_H
#define MGNODE_H

#include "mgftreeitem.h"
#include "models/MGFAnimationTableModel.h"
#include "models/MGFMeshTableModel.h"
#include "models/MGFMaterialTableModel.h"
#include "models/MGFNodeTreeModel.h"
#include "utils/configfile.h"

#include <vector>
#include <functional>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreMesh.h>
#include <OgreMaterial.h>

class MGNode
{
public:
    MGNode(const MGFTreeItem& nodeFile, Ogre::SceneManager* sceneMgr);

    Ogre::SceneNode* SceneNode() const { return m_Root; }

    MGFAnimationTableModel* GetAnimationTableModel() { return &m_AnimationTableModel; }
    MGFMeshTableModel* GetMeshTableModel() { return &m_MeshTableModel; }
    MGFMaterialTableModel* GetMaterialTableModel() { return &m_MaterialTableModel; }
    MGFNodeTreeModel* GetNodeTreeModel() { return &m_NodeTreeModel; }

private:
    const MGFTreeItem& m_SourceFile;

    Ogre::SceneNode* m_Root = nullptr;

    MGFAnimationTableModel m_AnimationTableModel;
    MGFMeshTableModel m_MeshTableModel;
    MGFMaterialTableModel m_MaterialTableModel;
    MGFNodeTreeModel m_NodeTreeModel;

private:

    void LoadScene(const MGFTreeItem& mgnodeFile);
    void LoadMGModel(const MGFTreeItem& mgmodelFile);

    void CreateSceneNode(Ogre::SceneNode* parent, const std::function<ConfigSection()>& func);
    void CreateSceneNode(Ogre::SceneNode* parent, const pugi::xml_node& xmlnode, const std::unordered_map<std::string, Ogre::MeshPtr> meshes);

    static Ogre::Vector3 StrToVector(const std::string_view str);
    static Ogre::Quaternion StrToQuat(const std::string_view axis, const std::string_view angle);
};

#endif // MGNODE_H
