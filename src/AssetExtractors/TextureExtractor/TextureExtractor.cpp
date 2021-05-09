#include "TextureExtractor.h"

#include "MGF/File.h"
#include "MGF/Structures/Tif.h"
#include "MGF/Factories/ImageFactory.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <s3tc/s3tc.h>

#include <QImage>

using namespace AssetExtractor;

bool TextureExtractor::Extract(const MGF::File& sourceFile, const std::filesystem::path& destinationPath)
{
	ImageWriter.setFileName(destinationPath.u8string().c_str());

	uint32_t width, height, flags, size;
	const unsigned char* data = nullptr;

	if (sourceFile.GetArchiveVersion() == MGF::Version::MechAssault)
	{
		MA1_TIF_FILE tif;
		MGF::Factories::ImageFactory::Deserialize(sourceFile, tif);

		width = tif.header.cWidth.imageWidth;
		height = tif.header.cHeight.imageHeight;
		flags = tif.header.cFlags.flags;
		size = tif.header.cSize.imageSize;
		data = reinterpret_cast<const unsigned char*>(tif.pixels);
	}
	else
	{
		MA2_TIF_FILE tif;
		MGF::Factories::ImageFactory::Deserialize(sourceFile, tif);

		width = tif.header.cWidth.imageWidth;
		height = tif.header.cHeight.imageHeight;
		flags = tif.header.cFlags.flags;
		size = tif.header.cSize.imageSize;
		data = reinterpret_cast<const unsigned char*>(tif.pixels);
	}


	if (uint8_t compressed_format_bits = (flags >> 8) & 0x0F; compressed_format_bits != 0)
	{
		std::vector<unsigned long> decompressed(width * height);
		switch (compressed_format_bits)
		{
		case 1: BlockDecompressImageDXT1(width, height, data, decompressed.data()); break;
		case 3: BlockDecompressImageDXT5(width, height, data, decompressed.data()); break;
		case 5: BlockDecompressImageDXT5(width, height, data, decompressed.data()); break;
		}

		QImage image(reinterpret_cast<const unsigned char*>(decompressed.data()), width, height, QImage::Format::Format_RGBA8888);
		ImageWriter.write(image);
	}
	else
	{
		const auto [format, bytesPerPixel] = [](uint8_t uncompressed_format_bits)
		{
			switch (uncompressed_format_bits)
			{
			case 0: return std::make_pair(QImage::Format::Format_ARGB32, 4);
			case 1: return std::make_pair(QImage::Format::Format_RGB16, 2);
			case 3: return std::make_pair(QImage::Format::Format_ARGB4444_Premultiplied, 2);
			case 5: return std::make_pair(QImage::Format::Format_Alpha8, 1);
			case 7: return std::make_pair(QImage::Format::Format_Grayscale8, 1);
			}
		}(flags & 0x0F);

		QImage image(data, width, height, (size / height), format, nullptr, nullptr);
		ImageWriter.write(image);
	}

	delete[] data;
	return true;
}
