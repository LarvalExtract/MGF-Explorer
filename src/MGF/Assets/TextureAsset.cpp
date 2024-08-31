#include "TextureAsset.h"
#include "../../Render/TextureLibrary.h"

using namespace MGF::Asset;

TextureAsset::TextureAsset(const File& file) :
	AssetBase(file, EAssetType::Texture),
	mTexture(MGF::Render::TextureLibrary::Get().GetTexture(file))
{
	TextureDetails.Version = file.ArchiveVersion;
	TextureDetails.Width = mTexture->width();
	TextureDetails.Height = mTexture->height();
	TextureDetails.Depth = mTexture->depth();
	TextureDetails.Mips = mTexture->mipLevels();
	TextureDetails.Frames = mTexture->layers();
	TextureDetails.Format = mTexture->format();
	TextureDetails.Size = 0;
}