#include "ImageFactory.h"

using namespace MGF::Factories;

void ImageFactory::Deserialize(const MGF::File& tif, MA2_TIF_FILE& image)
{
	tif.Read(reinterpret_cast<char*>(&image.header), 0, sizeof(image.header));

	const size_t length = image.header.cBits.length - 8;
	image.pixels = new char[length];

	tif.Read(image.pixels, sizeof(image.header), length);
}

void MGF::Factories::ImageFactory::Deserialize(const MGF::File& tif, MA1_TIF_FILE& image)
{
	tif.Read(reinterpret_cast<char*>(&image.header), 0, sizeof(image.header));

	const size_t length = image.header.cBits.length - 8;
	image.pixels = new char[length];

	tif.Read(image.pixels, sizeof(image.header), length);
}
