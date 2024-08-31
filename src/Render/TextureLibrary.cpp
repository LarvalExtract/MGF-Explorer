#include "TextureLibrary.h"
#include "MGF/File.h"
#include "MGF/Factories/ImageFactory.h"

#include <QTexture>
#include <QTextureImageDataGenerator>
#include <QTextureDataUpdate>

using namespace MGF::Render;
using namespace Qt3DRender;

TextureLibrary::~TextureLibrary()
{
	for (auto& [key, tex] : mTextureLibrary)
	{
	//	tex->setParent((Qt3DCore::QNode*)nullptr);
	}
}

TextureLibrary& TextureLibrary::Get()
{
	static TextureLibrary instance;
	return instance;
}

QAbstractTexture* TextureLibrary::GetTexture(const MGF::File& sourceFile)
{
	const uint32_t key = sourceFile.FilepathHash;

	if (!mTextureLibrary.contains(key))
	{
		mTextureLibrary.insert(std::make_pair(sourceFile.FilepathHash, CreateTexture(sourceFile)));
	}

	return mTextureLibrary.at(key);
}

QOpenGLTexture::Target DetermineTarget(uint32_t depth, uint32_t frames)
{
	if (depth == 1 && frames == 1)
		return QOpenGLTexture::Target::Target2D;
	if (depth == 1 && frames == 6 || depth == 6 && frames == 1)
		return QOpenGLTexture::Target::TargetCubeMap;
	if (depth > 1 && depth != 6 && frames == 1)
		return QOpenGLTexture::Target::Target3D;
	if (depth == 1 && frames > 1 && frames != 6)
		return QOpenGLTexture::Target::Target2DArray;
}

QOpenGLTexture::TextureFormat DetermineFormat(uint32_t flags)
{
	uint8_t uncompressed_format_bits = flags & 0x0F;
	uint8_t compressed_format_bits = (flags >> 8) & 0x0F;

	switch (compressed_format_bits)
	{
	case 1: return QOpenGLTexture::TextureFormat::RGBA_DXT1;
	case 3: return QOpenGLTexture::TextureFormat::RGBA_DXT3;
	case 5: return QOpenGLTexture::TextureFormat::RGBA_DXT5;
	}

	switch (uncompressed_format_bits)
	{
	case 0: return QOpenGLTexture::TextureFormat::RGBA8_UNorm;
	case 1: return QOpenGLTexture::TextureFormat::R5G6B5;
	case 3: return QOpenGLTexture::TextureFormat::RGBA4;
	case 5: return QOpenGLTexture::TextureFormat::R8_UNorm;
	case 7: return QOpenGLTexture::TextureFormat::R16U;
	}
}

QOpenGLTexture::PixelFormat DeterminePixelFormat(uint32_t flags)
{
	uint8_t uncompressed_format_bits = flags & 0x0F;
	uint8_t compressed_format_bits = (flags >> 8) & 0x0F;

	if (compressed_format_bits)
		return QOpenGLTexture::PixelFormat::RGBA;

	switch (uncompressed_format_bits)
	{
	case 0: return QOpenGLTexture::PixelFormat::BGRA;
	case 1: return QOpenGLTexture::PixelFormat::RGB;
	case 3: return QOpenGLTexture::PixelFormat::BGRA;
	case 5: return QOpenGLTexture::PixelFormat::Luminance;
	case 7: return QOpenGLTexture::PixelFormat::Luminance;
	}
}

QOpenGLTexture::PixelType DeterminePixelType(uint32_t flags)
{
	uint8_t uncompressed_format_bits = flags & 0x0F;
	uint8_t compressed_format_bits = (flags >> 8) & 0x0F;

	if (compressed_format_bits)
		return QOpenGLTexture::UInt8;

	switch (uncompressed_format_bits)
	{
	case 0: return QOpenGLTexture::UInt32_RGBA8_Rev;
	case 1: return QOpenGLTexture::UInt16_R5G6B5;
	case 3: return QOpenGLTexture::UInt16_RGBA4_Rev;
	case 5: return QOpenGLTexture::UInt8;
	case 7: return QOpenGLTexture::UInt16;
	}
}

int GetBlockSize(uint32_t flags)
{
	switch (DetermineFormat(flags))
	{
	case QOpenGLTexture::TextureFormat::RGBA_DXT1: return 8;
	case QOpenGLTexture::TextureFormat::RGBA_DXT3: return 16;
	case QOpenGLTexture::TextureFormat::RGBA_DXT5: return 16;
	}

	return 4;
}

int PixelSize(QOpenGLTexture::PixelType pt)
{
	switch (pt)
	{
	case QOpenGLTexture::UInt8: 
		return 1;
	case QOpenGLTexture::UInt16: 
	case QOpenGLTexture::UInt16_R5G6B5:
	case QOpenGLTexture::UInt16_RGBA4_Rev: 
		return 2;
	case QOpenGLTexture::UInt32_RGBA8_Rev:
		return 4;
	}
}

class MATextureImageDataGenerator final : public QTextureImageDataGenerator
{
public:
	MATextureImageDataGenerator(uint32_t width, uint32_t height, uint32_t depth, uint32_t flags, uint32_t mips, uint32_t frames, int face, int mipLevel, const char* pixels)
		: mWidth(width)
		, mHeight(height)
		, mDepth(depth)
		, mFlags(flags)
		, mMips(mips)
		, mFrames(frames)
		, mFace(face)
		, mMipLevel(mipLevel)
		, mPixels(pixels)
	{
		
	}

	QT3D_FUNCTOR(MATextureImageDataGenerator);

	QTextureImageDataPtr operator()() override
	{
		mWidth = std::max(mWidth >> mMipLevel, 1U);
		mHeight = std::max(mHeight >> mMipLevel, 1U);
		mDepth = std::max(mDepth >> mMipLevel, 1U);

		const bool isCompressed = (uint8_t)((mFlags >> 8) & 0x0F) > 0;
		const uint32_t paddedWidth = mWidth % 16 == 0 ? mWidth : mWidth + (16 - mWidth % 16);

		QTextureImageDataPtr image(new QTextureImageData);
		image->setWidth(paddedWidth); // Can't set GL_UNPACK_ROW_LENGTH with Qt, need to pad width to multiple of 16
		image->setHeight(mHeight);
		image->setMipLevels(mMips);
		image->setDepth(mDepth);
		image->setPixelFormat(DeterminePixelFormat(mFlags));
		image->setPixelType(DeterminePixelType(mFlags));
		image->setTarget(DetermineTarget(mDepth, mFrames));

		uint32_t bpp = PixelSize(image->pixelType());
		uint32_t faceSize = paddedWidth * mHeight * 1 * bpp;
		uint32_t mipSize = faceSize >> mMipLevel;

		uint32_t mipStart = 0;
		if (int mip = mMipLevel - 1; mip > -1)
		{
			mipStart = ((paddedWidth * bpp) >> mip) * ((mHeight * bpp) >> mip) * ((1 * bpp) >> mip);
		}

		uint32_t offset = mFace * faceSize + mipStart;
		uint32_t size = mipSize;

		if (isCompressed && DetermineFormat(mFlags) == QOpenGLTexture::TextureFormat::RGBA_DXT1)
			size >>= 1;

		QByteArray pixelData;
		pixelData.resize(size);
		memcpy(pixelData.data(), mPixels + offset, size);

		const int blockSize = isCompressed ? GetBlockSize(mFlags) : 0;
		image->setData(pixelData, blockSize, isCompressed);

		return image;
	}

	bool operator==(const QTextureImageDataGenerator&) const override
	{
		return false;
	}

private:
	unsigned int mWidth, mHeight, mDepth, mFlags, mMips, mFrames;
	const int mFace, mMipLevel;
	const char* mPixels;
};

class MATextureImage : public QAbstractTextureImage
{
public:
	MATextureImage(uint32_t width, uint32_t height, uint32_t depth, uint32_t flags, uint32_t mips, uint32_t frames, int face, int mipLevel, const char* pixels) 
		: mGenerator(new MATextureImageDataGenerator(width, height, depth, flags, mips, frames, face, mipLevel, pixels))
	{
		
	}

protected:
	QTextureImageDataGeneratorPtr dataGenerator() const override
	{
		return mGenerator;
	}

	QTextureImageDataGeneratorPtr mGenerator;
};

QAbstractTexture* TextureLibrary::CreateTexture(const MGF::File& sourceFile)
{
	QAbstractTexture* texture = nullptr;

	uint32_t width, height, flags, mips, depth, frames;
	char* pixels = nullptr;
	if (sourceFile.ArchiveVersion == MGF::Version::MechAssault2LW)
	{
		MA2_TIF_FILE description;
		MGF::Factories::ImageFactory::Deserialize(sourceFile, description, &pixels);

		width = description.header.cWidth.imageWidth;
		height = description.header.cHeight.imageHeight;
		flags = description.header.cFlags.flags;
		mips = description.header.cMips.numMips;
		depth = description.header.cDepth.imageDepth;
		frames = description.header.cFrames.numFrames;
	}
	else
	{
		MA1_TIF_FILE description;
		MGF::Factories::ImageFactory::Deserialize(sourceFile, description, &pixels);

		width = description.header.cWidth.imageWidth;
		height = description.header.cHeight.imageHeight;
		flags = description.header.cFlags.flags;
		mips = description.header.cMips.numMips;
		depth = 1;
		frames = 1;
	}

	switch (DetermineTarget(depth, frames))
	{
	case QOpenGLTexture::Target::Target2D:
		texture = new QTexture2D;
		break;
	case QOpenGLTexture::Target::TargetCubeMap:
		texture = new QTextureCubeMap;
		break;
	case QOpenGLTexture::Target::Target3D:
		texture = new QTexture3D;
		break;
	case QOpenGLTexture::Target::Target2DArray:
		texture = new QTexture2DArray;
		break;
	}

	for (int frame = 0; frame < frames; frame++)
	{
		for (int mip = 0; mip < mips; mip++)
		{
			auto textureImage = new MATextureImage(width, height, depth, flags, mips, frames, frame, mip, pixels);
			textureImage->setFace(QAbstractTexture::CubeMapFace((uint32_t)QAbstractTexture::CubeMapFace::CubeMapPositiveX + frame));
			textureImage->setMipLevel(mip);
			texture->addTextureImage(textureImage);
		}
	}

	texture->setWidth(width);
	texture->setHeight(height);
	texture->setMipLevels(mips);
	texture->setDepth(depth);
	texture->setFormat([flags]() -> QAbstractTexture::TextureFormat
	{
		switch (DetermineFormat(flags))
		{
		case QOpenGLTexture::TextureFormat::RGBA8_UNorm:
			return QAbstractTexture::TextureFormat::RGBA8_UNorm;
		case QOpenGLTexture::TextureFormat::R5G6B5:
			return QAbstractTexture::TextureFormat::R5G6B5;
		case QOpenGLTexture::TextureFormat::RGBA4:
			return QAbstractTexture::TextureFormat::RGBA4;
		case QOpenGLTexture::TextureFormat::R8_UNorm:
			return QAbstractTexture::TextureFormat::R8_UNorm;
		case QOpenGLTexture::TextureFormat::R16_UNorm:
			return QAbstractTexture::TextureFormat::R16_UNorm;
		case QOpenGLTexture::TextureFormat::RGBA_DXT1:
			return QAbstractTexture::TextureFormat::RGBA_DXT1;
		case QOpenGLTexture::TextureFormat::RGBA_DXT3:
			return QAbstractTexture::TextureFormat::RGBA_DXT3;
		case QOpenGLTexture::TextureFormat::RGBA_DXT5:
			return QAbstractTexture::TextureFormat::RGBA_DXT5;
		}
	}());

	return texture;
}


