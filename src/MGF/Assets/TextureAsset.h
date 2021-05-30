#pragma once

#include "AssetBase.h"
#include "AssetViewers/TextureViewer/Models/TextureDetailsTableModel.h"

#include <OgreTexture.h>

namespace MGF {	namespace Asset {

	class TextureAsset : public AssetBase
	{
	public:
		TextureAsset(const File& file);

		TextureViewer::Models::TextureDetailsTable TextureDetails;

		Ogre::TexturePtr OgreTexture;

	private:
		Ogre::PixelFormat DeterminePixelFormat();
		Ogre::TextureType DetermineTextureType();
	};

} }