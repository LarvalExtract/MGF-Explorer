#include "FileMenu.h"
#include "FileExtractor/Extractor.h"
#include "FileExtractor/FileExtractorDialog.h"

using namespace ArchiveViewer::ContextMenus;

void FileMenu::Initialise(QTreeView* treeView)
{
	connect(addAction("Extract..."), &QAction::triggered, [treeView]()
		{
			const auto& files = FileExtractor::Extractor::ToList(treeView->selectionModel()->selection().indexes());

			FileExtractor::FileExtractorDialog dialog;
			dialog.QueueFiles(files);
			dialog.exec();
		});
}

