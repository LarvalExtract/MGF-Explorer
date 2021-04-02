#pragma once

#include "FileExtractor/Extractor.h"

#include <QMenu>
#include <QTreeView>

namespace ArchiveViewer { namespace ContextMenus {

	class FileMenu : public QMenu
	{
		
	public:
		void Initialise(QTreeView* treeView);

	};

} }