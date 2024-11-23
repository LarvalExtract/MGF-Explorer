#include "MGFTexture.h"
#include "MGF/MGFArchive.h"
#include "Render/TextureLibrary.h"

MGFTexture::MGFTexture(const MGFFile& file) :
	MGFAsset(file),
	mTexture(MGF::Render::TextureLibrary::Get().GetTexture(file))
{
	TextureDetails.Version = file.MgfArchive.GetVersion();
	TextureDetails.Width = mTexture->width();
	TextureDetails.Height = mTexture->height();
	TextureDetails.Depth = mTexture->depth();
	TextureDetails.Mips = mTexture->mipLevels();
	TextureDetails.Frames = mTexture->layers();
	TextureDetails.Format = mTexture->format();
	TextureDetails.Size = 0;
}