#include "MATexture.h"
#include "MGFArchive.h"

#include <libmorton/morton.h>

enum TifHeaderOffset : unsigned char
{
	Flags = 33,
	Width = 45,
	Height = 57,
	Mips = 69,
	Size = 81,
	Frames = 93,
	Depth = 105,
	Data = 117
};

enum MGFTextureFormat : unsigned int
{
	BGRA8888 = 0,
	RGB565 = 1,
	BGRA4444 = 3,
	R8 = 5,
	BGRA5551 = 7
};

MATexture::MATexture(const MGFTreeNode& treeNode) :
	Texture2D(),
	depth(0),
	mips(0),
	frames(0),
	format(0),
	flags(0)
{
	char header[117];
	treeNode.archive.FileStream().seekg(treeNode.FileOffset() + 32, std::ios::beg);
	treeNode.archive.FileStream().read(header, 117);

	this->flags = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Flags]);
	this->width = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Width]);
	this->height = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Height]);
	this->mips = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Mips]);
	this->size = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Size]);
	this->frames = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Frames]);
	this->depth = *reinterpret_cast<unsigned int*>(&header[TifHeaderOffset::Depth]);

	unsigned char* pixels = new unsigned char[size];
	treeNode.archive.FileStream().seekg(treeNode.FileOffset() + 32 + TifHeaderOffset::Data);
	treeNode.archive.FileStream().read(reinterpret_cast<char*>(pixels), size);

	InitTexture(pixels);

	delete[] pixels;
}

MATexture::~MATexture()
{

}

void MATexture::InitTexture(unsigned char* pixels)
{
	Bind();

	char format		= (flags >> 0) & 0x0F;
	char compressed = (flags >> 8) & 0x0F;
	bool morton = format & 0xF0;

	if (morton == true)
	{

	}

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width + CalculateStride());

	// Texture is compressed (DXT1, DXT3 or DXT5)
	if (compressed > 0)
	{
		switch (compressed)
		{
		case 1:	UploadCompressedPixelData(CompressedTextureFormat::DXT1a, width, height, (width * height) / 2, pixels);	break;
		case 3: UploadCompressedPixelData(CompressedTextureFormat::DXT3, width, height, width * height, pixels);			break;
		case 5: UploadCompressedPixelData(CompressedTextureFormat::DXT5, width, height, width * height, pixels);			break;
		}
	}

	else
	{
		switch (format)
		{
		case MGFTextureFormat::BGRA8888:	UploadPixelData(PixelFormat::RGBA, width, height, PixelFormat::BGRA, pixels, GL_UNSIGNED_BYTE);		break;
		case MGFTextureFormat::RGB565:		UploadPixelData(PixelFormat::RGB, width, height, PixelFormat::RGB, pixels, GL_UNSIGNED_SHORT_5_6_5);			break;
		case MGFTextureFormat::BGRA4444:	UploadPixelData(PixelFormat::RGBA, width, height, PixelFormat::BGRA, pixels, GL_UNSIGNED_SHORT_4_4_4_4_REV);	break;
		case MGFTextureFormat::R8:			UploadPixelData(PixelFormat::Red, width, height, PixelFormat::Red, pixels, GL_UNSIGNED_BYTE);					break;
		case MGFTextureFormat::BGRA5551:	UploadPixelData(PixelFormat::RGBA, width, height, PixelFormat::BGRA, pixels, GL_UNSIGNED_SHORT_5_5_5_1);		break;
		}
	}
}

unsigned int MATexture::CalculateStride()
{
	unsigned int actualRowLength = size / height;
	actualRowLength /= frames;
	actualRowLength /= depth;

	char bpp;
	switch (flags & 0x0000000F)
	{
	case BGRA8888:	bpp = 4; break;
	case RGB565:	bpp = 2; break;
	case BGRA4444:	bpp = 2; break;
	case R8:		bpp = 1; break;
	case BGRA5551:	bpp = 2; break;
	}

	unsigned int expectedRowLength = width * bpp;

	return (actualRowLength - expectedRowLength) / bpp;
}
