#pragma once

#include "FileMenu.h"
#include "AssetExtractors/TextureExtractor/TextureExtractor.h"

namespace ArchiveViewer { namespace ContextMenus {

	class TextureFileMenu : public FileMenu
	{
	public:
		void Initialise(QTreeView* treeView) override;

	private:
		AssetExtractor::TextureExtractor TextureExtractor;
	};

} }