#include "Texture.h"

#include "MGF/Factories/ImageFactory.h"

#include <OgreTextureManager.h>

using namespace MGF::Asset;

Texture::Texture(const File& file) :
	AssetBase(file, EAssetType::Texture)
{
	char* pixels = nullptr;

	if (file.GetArchiveVersion() == MGF::Version::MechAssault2LW)
	{
		MA2_TIF_FILE tif;
		Factories::ImageFactory::Deserialize(file, tif);

		Version = MGF::Version::MechAssault2LW;
		Width = tif.header.cWidth.imageWidth;
		Height = tif.header.cHeight.imageHeight;
		Flags = tif.header.cFlags.flags;
		Mips = tif.header.cMips.numMips - 1;
		Size = tif.header.cSize.imageSize;
		Frames = 1;// header.cFrames.numFrames;
		Depth = tif.header.cDepth.imageDepth;

		pixels = tif.pixels;
	}
	else
	{
		MA1_TIF_FILE tif;
		Factories::ImageFactory::Deserialize(file, tif);

		Version = MGF::Version::MechAssault;
		Width = tif.header.cWidth.imageWidth;
		Height = tif.header.cHeight.imageHeight;
		Flags = tif.header.cFlags.flags;
		Mips = tif.header.cMips.numMips - 1;
		Size = tif.header.cSize.imageSize;
		Frames = 1;
		Depth = 1;

		pixels = tif.pixels;
	}

	size_t actualWidth = (Width % 16 == 0) ? Width : Width + (16 - (Width % 16));
	size_t actualSize = Ogre::Image::calculateSize(Mips, Frames, actualWidth, Height, Depth, DeterminePixelFormat());

	Ogre::DataStreamPtr stream;
	stream.reset(new Ogre::MemoryDataStream(pixels, actualSize));

	Ogre::Image image;
	image.loadRawData(stream, actualWidth, Height, Depth, DeterminePixelFormat(), Frames, Mips);

	auto& textureManager = Ogre::TextureManager::getSingleton();
	OgreTexture = textureManager.loadImage(std::to_string(file.GUID()), "General", image, DetermineTextureType());

	delete[] pixels;
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