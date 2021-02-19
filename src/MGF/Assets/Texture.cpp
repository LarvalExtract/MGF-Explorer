#include "Texture.h"

#include "MGF/Structures/Tif.h"

#include <OgreTextureManager.h>

using namespace MGF::Asset;

Texture::Texture(const File& file) :
	AssetBase(file, EAssetType::Texture)
{
	std::vector<uint8_t> pixels;

	if (file.GetArchiveVersion() == MGF::Version::MechAssault2LW)
	{
		MA2_TIF_HEADER header;
		file.Read(reinterpret_cast<char*>(&header), 0, sizeof(header));

		Version = MGF::Version::MechAssault2LW;
		Width = header.cWidth.imageWidth;
		Height = header.cHeight.imageHeight;
		Flags = header.cFlags.flags;
		Mips = header.cMips.numMips - 1;
		Size = header.cSize.imageSize;
		Frames = header.cFrames.numFrames;
		Depth = header.cDepth.imageDepth;

		pixels.resize(header.cBits.length);
		file.Read(reinterpret_cast<char*>(pixels.data()), sizeof(MA2_TIF_HEADER), header.cBits.length - 8);
	}
	else
	{
		MA1_TIF_HEADER header;
		file.Read(reinterpret_cast<char*>(&header), 0, sizeof(header));

		Version = MGF::Version::MechAssault;
		Width = header.cWidth.imageWidth;
		Height = header.cHeight.imageHeight;
		Flags = header.cFlags.flags;
		Mips = header.cMips.numMips - 1;
		Size = header.cSize.imageSize;
		Frames = 1;
		Depth = 1;

		pixels.resize(header.cBits.length);
		file.Read(reinterpret_cast<char*>(pixels.data()), sizeof(MA2_TIF_HEADER), header.cBits.length - 8);
	}

	size_t actualWidth = (Width % 16 == 0) ? Width : Width + (16 - (Width % 16));

	Ogre::DataStreamPtr stream;
	stream.reset(new Ogre::MemoryDataStream(pixels.data(), pixels.size()));

	Ogre::Image image;
	image.loadRawData(stream, actualWidth, Height, Depth, DeterminePixelFormat(), Frames, Mips);

	auto& textureManager = Ogre::TextureManager::getSingleton();
	OgreTexture = textureManager.loadImage(std::to_string(file.GUID()), "General", image, DetermineTextureType());
}

Ogre::PixelFormat Texture::DeterminePixelFormat()
{
	uint8_t uncompressed_format_bits = Flags & 0x0F;
	uint8_t compressed_format_bits = (Flags >> 8) & 0x0F;

	switch (compressed_format_bits)
	{
	case 1: return Ogre::PixelFormat::PF_DXT1;
	case 3: return Ogre::PixelFormat::PF_DXT3;
	case 5: return Ogre::PixelFormat::PF_DXT5;
	}

	switch (uncompressed_format_bits)
	{
	case 0: return Ogre::PixelFormat::PF_A8R8G8B8;
	case 1: return Ogre::PixelFormat::PF_R5G6B5;
	case 3: return Ogre::PixelFormat::PF_A4R4G4B4;
	case 5: return Ogre::PixelFormat::PF_A8;
	case 7: return Ogre::PixelFormat::PF_L8;
	}
}

Ogre::TextureType Texture::DetermineTextureType()
{
	if (Depth > 1)
	{
		if (Depth == 6)
			return Ogre::TextureType::TEX_TYPE_CUBE_MAP;
		else
			return Ogre::TextureType::TEX_TYPE_3D;
	}

	if (Frames > 1)
		return Ogre::TextureType::TEX_TYPE_2D_ARRAY;

	return Ogre::TextureType::TEX_TYPE_2D;
}