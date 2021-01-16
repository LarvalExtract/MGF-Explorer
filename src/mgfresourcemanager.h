#ifndef MGFRESOURCEMANAGER_H
#define MGFRESOURCEMANAGER_H

#include "mgf/File.h"
#include "mgf/mgtexture.h"
#include "mgf/mgnode.h"

#include <QString>
#include <QHash>

class MGFResourceManager
{
public:
    const QString& GetTextData(const MGF::File& item);
    MGTexture& GetTexture(const MGF::File& item);
    MGNode& GetModel(const MGF::File& item, Ogre::SceneManager* sceneManager);

    void Clear();

private:
    QHash<uint64_t, QString> m_MapTextData;
    std::unordered_map<std::uint64_t, MGTexture> m_MapTextures;
    std::unordered_map<std::uint64_t, MGNode> m_MapModels;

private:
    void LoadTextData(MGF::File& item);
    void LoadTexture(std::uint64_t key, MGF::File& item);
    void LoadModel(std::uint64_t key, MGF::File& item, Ogre::SceneManager* sceneManager);
};

#endif // MGFRESOURCEMANAGER_H
