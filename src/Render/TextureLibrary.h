#pragma once

#include <unordered_map>
#include <memory>

#include <QAbstractTexture>

namespace MGF
{
	class File;
}

namespace MGF::Render
{
	class TextureLibrary final
	{
	public:
		~TextureLibrary();

		static TextureLibrary& Get();

		Qt3DRender::QAbstractTexture* GetTexture(const MGF::File& sourceFile);

	private:
		TextureLibrary() {}

		Qt3DRender::QAbstractTexture* CreateTexture(const MGF::File& sourceFile);
		std::unordered_map<uint32_t, Qt3DRender::QAbstractTexture*> mTextureLibrary;
	};
}