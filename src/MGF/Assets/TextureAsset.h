#pragma once

#include "AssetBase.h"
#include "AssetViewers/TextureViewer/Models/TextureDetailsTableModel.h"

namespace Qt3DRender { class QAbstractTexture; }

namespace MGF {	namespace Asset {

	class TextureAsset : public AssetBase
	{
	public:
		TextureAsset(const File& file);

		TextureViewer::Models::TextureDetailsTable TextureDetails;
		Qt3DRender::QAbstractTexture* mTexture = nullptr;
	};

} }