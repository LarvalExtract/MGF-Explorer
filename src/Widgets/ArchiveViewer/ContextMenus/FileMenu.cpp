#include "FileMenu.h"
#include "Windows/FileExtractorWindow/Extractor.h"

#include <QFileDialog>

using namespace ContextMenus;

FileMenu::FileMenu()
{
	connect(
		addAction("Extract"),
		&QAction::triggered,
		[this]()
		{
			const auto selection = SelectedItems();
			const auto file = selection.at(0).mgfItem;
			const auto fileName = QString(file->FilePath().filename().u8string().c_str());

			if (const auto destination = QFileDialog::getSaveFileName(nullptr, QString("Extract %1").arg(fileName), fileName); !destination.isEmpty())
			{
				std::filesystem::path dest(destination.toLatin1().data());
				std::vector<char> fileBuffer(16384);

				FileExtractor::Extractor::WriteFile(*file, dest, fileBuffer);
			}
		}
	);
}

