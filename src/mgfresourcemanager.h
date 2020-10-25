#ifndef MGFRESOURCEMANAGER_H
#define MGFRESOURCEMANAGER_H

#include "mgf/mgftreeitem.h"
#include "mgf/mgtexture.h"
#include "mgf/mgnode.h"

#include <QString>
#include <QHash>

class MGFResourceManager
{
public:
    const QString& GetTextData(const MGFTreeItem& item);
    MGTexture& GetTexture(const MGFTreeItem& item);
    MGNode& GetModel(const MGFTreeItem& item, Ogre::SceneManager* sceneManager);

    void Clear();

private:
    QHash<uint64_t, QString> m_MapTextData;
    std::unordered_map<std::uint64_t, MGTexture> m_MapTextures;
    std::unordered_map<std::uint64_t, MGNode> m_MapModels;

private:
    void LoadTextData(MGFTreeItem& item);
    void LoadTexture(std::uint64_t key, MGFTreeItem& item);
    void LoadModel(std::uint64_t key, MGFTreeItem& item, Ogre::SceneManager* sceneManager);
};

#endif // MGFRESOURCEMANAGER_H
