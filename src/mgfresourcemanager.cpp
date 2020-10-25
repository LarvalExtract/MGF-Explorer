#include "mgfresourcemanager.h"
#include "mgf/mgfarchive.h"

#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>

const QString &MGFResourceManager::GetTextData(const MGFTreeItem &item)
{
    uint64_t key = item.GUID();

    if (m_MapTextData.find(key) == m_MapTextData.end())
        LoadTextData(const_cast<MGFTreeItem&>(item));

    return m_MapTextData[key];
}

MGTexture& MGFResourceManager::GetTexture(const MGFTreeItem &item)
{
    auto key = item.GUID();

    if (m_MapTextures.find(key) == m_MapTextures.end())
        LoadTexture(key, const_cast<MGFTreeItem&>(item));

    return m_MapTextures.at(key);
}

MGNode& MGFResourceManager::GetModel(const MGFTreeItem &item, Ogre::SceneManager* sceneManager)
{
    auto key = item.GUID();

    if (m_MapModels.find(key) == m_MapModels.end())
        LoadModel(key, const_cast<MGFTreeItem&>(item), sceneManager);

    return m_MapModels.at(key);
}

void MGFResourceManager::Clear()
{
    m_MapTextData.clear();
    m_MapTextures.clear();
    m_MapModels.clear();

    Ogre::MaterialManager::getSingleton().destroyAllResourcePools();
    Ogre::TextureManager::getSingleton().destroyAllResourcePools();
    Ogre::MeshManager::getSingleton().destroyAllResourcePools();
}

void MGFResourceManager::LoadTextData(MGFTreeItem &item)
{
    std::string buffer;
    item.LoadBuffer(buffer);
    // append null-terminator to buffer
    buffer.push_back(0);

    m_MapTextData.insert(item.GUID(), buffer.data());
}

void MGFResourceManager::LoadTexture(std::uint64_t key, MGFTreeItem &item)
{
    m_MapTextures.emplace(key, MGTexture(item));
}

void MGFResourceManager::LoadModel(std::uint64_t key, MGFTreeItem &item, Ogre::SceneManager* sceneManager)
{
    m_MapModels.try_emplace(key, item, sceneManager);
}
