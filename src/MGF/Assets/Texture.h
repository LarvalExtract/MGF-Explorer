#pragma once

#include "AssetBase.h"
#include "mgf/VersionConstants.h"

#include <OgreTexture.h>

namespace MGF {	namespace Asset {

	class Texture : public AssetBase
	{
	public:
		Texture(const File& file);

		inline MGF::Version Version() const { return this->Version; }

	private:
		MGF::Version Version;
		std::uint32_t Width;
		std::uint32_t Height;
		std::uint32_t Depth;
		std::uint32_t Frames;
		std::uint32_t Mips;
		std::uint32_t Size;
		std::uint32_t Flags;
		Ogre::TexturePtr OgreTexture;

		Ogre::PixelFormat DeterminePixelFormat();
		Ogre::TextureType DetermineTextureType();
	};

} }