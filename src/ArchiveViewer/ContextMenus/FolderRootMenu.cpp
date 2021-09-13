#include "FolderRootMenu.h"
#include "FileExtractorWindow/Extractor.h"
#include "FileExtractorWindow/FileExtractorDialog.h"

#include "MGF/Archive.h"

using namespace ArchiveViewer::ContextMenus;

FolderRootMenu::FolderRootMenu()
{
	connect(addAction("Extract all..."), &QAction::triggered, [this]()
		{
			const auto model = static_cast<const MGF::Archive*>(SelectionModel->model());
			const auto selection = AllItems(*model);

			FileExtractor::FileExtractorDialog dialog;
			dialog.QueueFiles(selection);
			dialog.exec();
		});
}