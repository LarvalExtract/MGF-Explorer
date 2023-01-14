#pragma once

#include <unordered_map>
#include <memory>

#include <QAbstractTexture>

namespace MGF
{
	class File;
}

namespace MA
{
	class TextureLibrary final
	{
	public:
		~TextureLibrary();

		Qt3DRender::QAbstractTexture* GetTexture(const MGF::File& sourceFile);

	private:
		Qt3DRender::QAbstractTexture* CreateTexture(const MGF::File& sourceFile);
		std::unordered_map<uint32_t, Qt3DRender::QAbstractTexture*> mTextureLibrary;
	};
}