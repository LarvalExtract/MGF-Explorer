#pragma once

#include "FileMenu.h"
#include "AssetExtractors/TextureExtractor/TextureExtractor.h"

namespace ContextMenus
{
	class TextureFileMenu : public FileMenu
	{
	public:
		TextureFileMenu();

	private:
		AssetExtractor::TextureExtractor TextureExtractor;
	};
} 