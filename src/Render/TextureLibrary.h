#pragma once

#include <unordered_map>
#include <memory>

#include <QAbstractTexture>

class MGFFile;

namespace MGF::Render
{
	class TextureLibrary final
	{
	public:
		~TextureLibrary();

		static TextureLibrary& Get();

		Qt3DRender::QAbstractTexture* GetTexture(const MGFFile& sourceFile);

	private:
		TextureLibrary() {}

		Qt3DRender::QAbstractTexture* CreateTexture(const MGFFile& sourceFile);
		std::unordered_map<uint32_t, Qt3DRender::QAbstractTexture*> mTextureLibrary;
	};
}