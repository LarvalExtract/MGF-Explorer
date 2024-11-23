#pragma once

#include "MGF/Assets/MGFAsset.h"

#include <QWidget>

class IAssetViewerWidget : public QWidget
{
public:
    void SetAsset(std::shared_ptr<MGFAsset> asset)
    {
        AssetPtr = asset;

        OnAssetLoaded(*AssetPtr);
    }

protected:
    virtual void OnAssetLoaded(MGFAsset& AssetRef) = 0;

    std::shared_ptr<MGFAsset> AssetPtr;
};
