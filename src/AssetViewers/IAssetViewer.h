#pragma once

#include "ResourceManager/ResourceManager.h"

class IAssetViewer
{
public:
	virtual void LoadAsset(MGF::AssetPtr asset);

protected:
	MGF::AssetPtr Asset;
};