#include "FolderMenu.h"
#include "Windows/FileExtractorWindow/Extractor.h"
#include "Windows/FileExtractorWindow/FileExtractorDialog.h"

using namespace ContextMenus;

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