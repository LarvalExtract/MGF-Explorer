#include "TextureAsset.h"

#include "MGF/Factories/ImageFactory.h"

#include <OgreTextureManager.h>
#include <libmorton/libmorton/morton2D.h>

using namespace MGF::Asset;

TextureAsset::TextureAsset(const File& file) :
	AssetBase(file, EAssetType::Texture)
{
	char* pixels = nullptr;

	if (file.ArchiveVersion == MGF::Version::MechAssault2LW)
	{
		MA2_TIF_FILE tif;
		Factories::ImageFactory::Deserialize(file, tif);

		TextureDetails.Version = MGF::Version::MechAssault2LW;
		TextureDetails.Width = tif.header.cWidth.imageWidth;
		TextureDetails.Height = tif.header.cHeight.imageHeight;
		TextureDetails.Flags = tif.header.cFlags.flags;
		TextureDetails.Mips = tif.header.cMips.numMips;
		TextureDetails.Size = tif.header.cSize.imageSize;
		TextureDetails.Frames = tif.header.cFrames.numFrames;
		TextureDetails.Depth = tif.header.cDepth.imageDepth;

		pixels = tif.pixels;
	}
	else
	{
		MA1_TIF_FILE tif;
		Factories::ImageFactory::Deserialize(file, tif);

		TextureDetails.Version = MGF::Version::MechAssault;
		TextureDetails.Width = tif.header.cWidth.imageWidth;
		TextureDetails.Height = tif.header.cHeight.imageHeight;
		TextureDetails.Flags = tif.header.cFlags.flags;
		TextureDetails.Mips = tif.header.cMips.numMips;
		TextureDetails.Size = tif.header.cSize.imageSize;
		TextureDetails.Frames = 1;
		TextureDetails.Depth = 1;

		pixels = tif.pixels;
	}

	const auto width = TextureDetails.Width;
	const auto height = TextureDetails.Height;
	const auto flags = TextureDetails.Flags;
	const auto depth = 1;// TextureDetails.Depth;
	const auto mips = 0;// TextureDetails.Mips - 1;
	const auto frames = 1;// TextureDetails.Frames;
	const auto pf = DeterminePixelFormat();

	size_t actualWidth = (width % 16 == 0) ? width : width + (16 - (width % 16));
	size_t actualSize = Ogre::Image::calculateSize(mips, frames, actualWidth, height, depth, pf);

	Ogre::DataStreamPtr stream;
	stream.reset(new Ogre::MemoryDataStream(pixels, actualSize));

	auto& textureManager = Ogre::TextureManager::getSingleton();
	OgreTexture = textureManager.loadRawData(std::to_string(file.FilepathHash), "General", stream, actualWidth, height, pf, DetermineTextureType(), 0);

	delete[] pixels;
}

Ogre::PixelFormat TextureAsset::DeterminePixelFormat()
{
	uint8_t uncompressed_format_bits = TextureDetails.Flags & 0x0F;
	uint8_t compressed_format_bits = (TextureDetails.Flags >> 8) & 0x0F;

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
	case 7: return Ogre::PixelFormat::PF_L16;
	}
}

Ogre::TextureType TextureAsset::DetermineTextureType()
{
	if (TextureDetails.Depth > 1)
	{
		if (TextureDetails.Depth == 6)
			return Ogre::TextureType::TEX_TYPE_CUBE_MAP;
		else
			return Ogre::TextureType::TEX_TYPE_3D;
	}

	if (TextureDetails.Frames > 1)
		return Ogre::TextureType::TEX_TYPE_2D_ARRAY;

	return Ogre::TextureType::TEX_TYPE_2D;
}