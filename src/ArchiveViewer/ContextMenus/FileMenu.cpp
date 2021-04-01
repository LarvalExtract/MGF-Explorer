#include "FileMenu.h"
#include "FileExtractor/Extractor.h"
#include "FileExtractor/FileExtractorDialog.h"
#include "Utilities/ContextProvider/ServiceProvider.h"

using namespace ArchiveViewer::ContextMenus;

FileMenu::FileMenu() :
	FileExtractorService(*ServiceProvider::Inject<FileExtractor::Extractor>())
{
	
}

void FileMenu::Initialise(QTreeView* treeView)
{
	TreeView = treeView;

	connect(addAction("Extract..."), &QAction::triggered, [this]()
		{
			const auto& files = FileExtractorService.ToList(TreeView->selectionModel()->selection().indexes());

			FileExtractor::FileExtractorDialog dialog;
			dialog.QueueFiles(files);
			dialog.exec();
		});
}

