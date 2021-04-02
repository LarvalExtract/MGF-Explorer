#include "FileMenu.h"
#include "FileExtractor/Extractor.h"
#include "FileExtractor/FileExtractorDialog.h"
#include "Utilities/ContextProvider/ServiceProvider.h"

using namespace ArchiveViewer::ContextMenus;

void FileMenu::Initialise(QTreeView* treeView)
{
	auto& FileExtractorService = *ServiceProvider::Inject<FileExtractor::Extractor>();

	connect(addAction("Extract..."), &QAction::triggered, [&]()
		{
			const auto& files = FileExtractorService.ToList(treeView->selectionModel()->selection().indexes());

			FileExtractor::FileExtractorDialog dialog;
			dialog.QueueFiles(files);
			dialog.exec();
		});
}

