#include "ResourceManager.h"

std::shared_ptr<MGF::Asset::AssetBase> ResourceManager::Get(const MGF::File& file)
{
    const auto id = file.GUID();

    if (Assets.find(id) == Assets.end())
    {
        Assets.emplace(id, Factory.Create(file));
    }

    return Assets.at(id);
}

