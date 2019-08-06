#include "Texture.h"

TextureBase::TextureBase(GLenum target) :
	target(target)
{
	glGenTextures(1, &textureID);
	
	Bind();

	SetWrapMode(WrapMode::Repeat);

	SetMinificationFilter(Filter::Linear);
	SetMagnificationFilter(Filter::Linear);
}

TextureBase::~TextureBase()
{
	glDeleteTextures(1, &textureID);
}

void TextureBase::Bind(unsigned int slot) const
{
	glBindTexture(target, textureID);
	glActiveTexture(GL_TEXTURE0 + slot);
}

void TextureBase::SetWrapMode(WrapMode mode)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
	glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
}

void TextureBase::SetMinificationFilter(Filter filter)
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
}

void TextureBase::SetMagnificationFilter(Filter filter)
{
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
}

void Texture2D::UploadPixelData(PixelFormat storageFormat, int width, int height, PixelFormat imageFormat, void* pixels, GLenum pixelDataDype)
{
	glTexImage2D(target, 0, static_cast<GLint>(storageFormat), width, height, 0, static_cast<GLint>(imageFormat), pixelDataDype, pixels);
}

void Texture2D::UploadCompressedPixelData(CompressedTextureFormat format, int width, int height, int size, void* pixels)
{
	glCompressedTexImage2D(target, 0, static_cast<GLenum>(format), width, height, 0, size, pixels);
}
