#include "mgfresourcemanager.h"
#include "mgf/Archive.h"

#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>

const QString &MGFResourceManager::GetTextData(const MGF::File &item)
{
    uint64_t key = item.GUID();

    if (m_MapTextData.find(key) == m_MapTextData.end())
        LoadTextData(const_cast<MGF::File&>(item));

    return m_MapTextData[key];
}

MGTexture& MGFResourceManager::GetTexture(const MGF::File &item)
{
    auto key = item.GUID();

    if (m_MapTextures.find(key) == m_MapTextures.end())
        LoadTexture(key, const_cast<MGF::File&>(item));

    return m_MapTextures.at(key);
}

MGNode& MGFResourceManager::GetModel(const MGF::File &item, Ogre::SceneManager* sceneManager)
{
    auto key = item.GUID();

    if (m_MapModels.find(key) == m_MapModels.end())
        LoadModel(key, const_cast<MGF::File&>(item), sceneManager);

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

void MGFResourceManager::LoadTextData(MGF::File &item)
{
    std::string buffer;
    item.LoadBuffer(buffer);
    // append null-terminator to buffer
    buffer.push_back(0);

    m_MapTextData.insert(item.GUID(), buffer.data());
}

void MGFResourceManager::LoadTexture(std::uint64_t key, MGF::File &item)
{
    m_MapTextures.emplace(key, MGTexture(item));
}

void MGFResourceManager::LoadModel(std::uint64_t key, MGF::File &item, Ogre::SceneManager* sceneManager)
{
    m_MapModels.try_emplace(key, item, sceneManager);
}
