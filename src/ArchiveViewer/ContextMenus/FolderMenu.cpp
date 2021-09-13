#include "FolderMenu.h"
#include "FileExtractorWindow/Extractor.h"
#include "FileExtractorWindow/FileExtractorDialog.h"

using namespace ArchiveViewer::ContextMenus;

FolderMenu::FolderMenu()
{
	connect(addAction("Extract folder..."), &QAction::triggered, [this]()
	{
		const auto selection = SelectedItems();
		
		FileExtractor::FileExtractorDialog dialog;
		dialog.QueueFiles(selection);
		dialog.exec();
	});
}