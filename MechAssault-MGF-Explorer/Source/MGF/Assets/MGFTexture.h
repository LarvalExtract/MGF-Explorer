#pragma once

#include "MGFAsset.h"
#include "Widgets/TextureViewer/Models/TextureDetailsTableModel.h"

namespace Qt3DRender { class QAbstractTexture; }

class MGFTexture : public MGFAsset
{
public:
	MGFTexture(const MGFFile& file);

	Models::TextureDetailsTable TextureDetails;
	Qt3DRender::QAbstractTexture* mTexture = nullptr;
};
