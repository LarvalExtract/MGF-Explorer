#include "AssetManager.h"

using namespace MGF;

Asset::AssetPtr AssetManager::Get(const MGF::File& file)
{
    const auto id = file.GUID();

    if (Assets.find(id) == Assets.end())
    {
        auto asset = Asset::AssetBase::Create(file);
        if (asset == nullptr)
        {
            return nullptr;
        }
        
        Assets.emplace(id, asset);
    }

    return Assets.at(id);
}

