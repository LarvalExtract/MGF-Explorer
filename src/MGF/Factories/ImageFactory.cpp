#include "ImageFactory.h"
#include "MGF/Deserializer.h"

#include <libmorton/libmorton/morton.h>

using namespace MGF::Factories;

void DeswizzlePixelData(uint32_t width, uint32_t height, uint32_t flags, char* &pixels);

void ImageFactory::Deserialize(const MGF::File& tif, MA2_TIF_FILE& image)
{
	MGF::Deserializer deserializer(tif);
	image = deserializer.Deserialize<MA2_TIF_FILE>();

	const size_t length = image.header.cBits.length - 8;
	image.pixels = new char[length];

	deserializer.ReadBytes(image.pixels, length, sizeof(image.header));

	if (image.header.cFlags.flags & 0x00000010)
	{
		DeswizzlePixelData(
			image.header.cWidth.imageWidth,
			image.header.cHeight.imageHeight,
			image.header.cFlags.flags,
			image.pixels
		);
	}
}

void MGF::Factories::ImageFactory::Deserialize(const MGF::File& tif, MA1_TIF_FILE& image)
{
	MGF::Deserializer deserializer(tif);
	image = deserializer.Deserialize<MA1_TIF_FILE>();

	const size_t length = image.header.cBits.length - 8;
	image.pixels = new char[length];

	deserializer.ReadBytes(image.pixels, length, sizeof(image.header));

	if (image.header.cFlags.flags & 0x00000010)
	{
		DeswizzlePixelData(
			image.header.cWidth.imageWidth,
			image.header.cHeight.imageHeight,
			image.header.cFlags.flags,
			image.pixels
		);
	}
}

template<typename T>
char* Deswizzle(char* const pixels, uint32_t width, uint32_t height)
{
	const auto src = reinterpret_cast<T*>(pixels);
	const auto dest = new T[width * height];

	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			const auto mortonIndex = libmorton::m2D_e_magicbits<uint32_t, uint32_t>(x, y);

			dest[y * width + x] = src[mortonIndex];
		}
	}

	delete[] src;

	return reinterpret_cast<char*>(dest);
}

void DeswizzlePixelData(uint32_t width, uint32_t height, uint32_t flags, char* &pixels)
{
	switch (flags & 0x0F)
	{
	case 0: pixels = Deswizzle<uint32_t>(pixels, width, height); break;
	case 1: pixels = Deswizzle<uint16_t>(pixels, width, height); break;
	case 3: pixels = Deswizzle<uint16_t>(pixels, width, height); break;
	case 5: pixels = Deswizzle<uint8_t>(pixels, width, height);	 break;
	case 7: pixels = Deswizzle<uint16_t>(pixels, width, height); break;
	}
}

