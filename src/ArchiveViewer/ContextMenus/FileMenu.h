#pragma once

#include <QMenu>
#include <QTreeView>

namespace ArchiveViewer { namespace ContextMenus {

	class FileMenu : public QMenu
	{
	public:
		virtual void Initialise(QTreeView* treeView);
	};

} }