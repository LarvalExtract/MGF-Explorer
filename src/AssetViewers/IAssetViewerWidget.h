#pragma once

#include "MGF/Assets/AssetBase.h"

class IAssetViewerWidget
{
public:
    virtual void LoadAsset(MGF::Asset::AssetPtr asset) = 0;
};
