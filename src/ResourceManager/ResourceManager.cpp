#include "ResourceManager.h"

#include "AssetFactory.h"

std::shared_ptr<MGF::Asset::AssetBase> ResourceManager::Get(const MGF::File& file)
{
    const auto id = file.GUID();

    if (Assets.find(id) == Assets.end())
    {
        auto asset = AssetFactory::Create(file);
        if (asset == nullptr)
        {
            return nullptr;
        }
        
        Assets.emplace(id, asset);
    }

    return Assets.at(id);
}

