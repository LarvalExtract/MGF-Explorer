#pragma once

#include <glad/glad.h>

enum class TextureTarget : GLenum
{
	Texture1D = GL_TEXTURE_1D,
	Texture1DArray = GL_TEXTURE_1D_ARRAY,
	Texture2D = GL_TEXTURE_2D,
	Texture2DArray = GL_TEXTURE_2D_ARRAY,
	Texture3D = GL_TEXTURE_3D,
	Cubemap = GL_TEXTURE_CUBE_MAP
};

enum class WrapMode : GLint
{
	Repeat = GL_REPEAT,
	RepeatMirrored = GL_MIRRORED_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class Filter : GLint
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class PixelFormat : GLint
{
	Red = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	BGR = GL_BGR,
	BGRA = GL_BGRA
};

enum class CompressedTextureFormat : GLenum
{
	DXT1 = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
	DXT1a = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
	DXT3 = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	DXT5 = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

enum class PixelDataType : GLenum
{
	UnsignedByte
};

class TextureBase
{
public:
	virtual ~TextureBase();

	void Bind(unsigned int slot = 0) const;

	void SetWrapMode(WrapMode mode);
	void SetMinificationFilter(Filter filter);
	void SetMagnificationFilter(Filter filter);

protected:
	TextureBase(GLenum target);

	GLuint textureID;
	GLenum target;
};

class Texture2D : public TextureBase
{
public:
	Texture2D() : TextureBase(GL_TEXTURE_2D), width(0), height(0) {}
	
	void UploadPixelData(PixelFormat storageFormat, int width, int height, PixelFormat imageFormat, void* pixels, GLenum pixelDataDype);
	void UploadCompressedPixelData(CompressedTextureFormat format, int width, int height, int size, void* pixels);

	inline unsigned int Width() const { return width; }
	inline unsigned int Height() const { return height; }

protected:
	unsigned int width;
	unsigned int height;
};

class Texture2DArray : public TextureBase
{
public:
	Texture2DArray() : TextureBase(GL_TEXTURE_2D_ARRAY) {}

private:
	void UploadPixelData(PixelFormat storageFormat, int width, int height, int depth, PixelFormat imageFormat, void* pixels, GLenum pixelDataDype);
	void UploadCompressedPixelData(CompressedTextureFormat format, int width, int height, int depth, int size, void* pixels);
};