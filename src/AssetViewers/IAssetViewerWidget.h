#pragma once

#include "MGF/Assets/AssetBase.h"

#include <QWidget>

class IAssetViewerWidget : public QWidget
{
public:
    virtual void LoadAsset(MGF::Asset::AssetPtr asset) = 0;
};
