#pragma once

#include "FileExtractor/Extractor.h"

#include <QMenu>
#include <QTreeView>

namespace ArchiveViewer { namespace ContextMenus {

	class FileMenu : public QMenu
	{
		
	public:
		FileMenu();

		void Initialise(QTreeView* treeView);

	protected:
		QTreeView* TreeView = nullptr;
		FileExtractor::Extractor& FileExtractorService;
	};

} }