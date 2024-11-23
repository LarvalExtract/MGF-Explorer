#include "FolderRootMenu.h"
#include "Windows/FileExtractorWindow/Extractor.h"
#include "Windows/FileExtractorWindow/FileExtractorDialog.h"

#include "MGF/MGFArchive.h"

using namespace ContextMenus;

FolderRootMenu::FolderRootMenu()
{
	connect(addAction("Extract all..."), &QAction::triggered, [this]()
		{
			const auto model = static_cast<const MGFArchive*>(SelectionModel->model());
			const auto selection = AllItems(*model);

			FileExtractor::FileExtractorDialog dialog;
			dialog.QueueFiles(selection);
			dialog.exec();
		});
}