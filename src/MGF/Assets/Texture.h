#pragma once

#include "AssetBase.h"
#include "MGF/VersionConstants.h"

#include <OgreTexture.h>

namespace MGF {	namespace Asset {

	class Texture : public AssetBase
	{
	public:
		Texture(const File& file);

		inline MGF::Version GetVersion() const { return Version; }
		inline std::uint32_t GetWidth() const { return Width; }
		inline std::uint32_t GetHeight() const { return Height; }
		inline std::uint32_t GetDepth() const { return Depth; }
		inline std::uint32_t GetFrames() const { return Frames; }
		inline std::uint32_t GetMips() const { return Mips; }
		inline std::uint32_t GetSize() const { return Size; }
		inline std::uint32_t GetFlags() const { return Flags; }
		Ogre::TexturePtr GetOgreTexture() const { return OgreTexture; }

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