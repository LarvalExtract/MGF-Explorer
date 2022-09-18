#include "TextureFileMenu.h"
#include "MGF/File.h"

#include <QFileDialog>

using namespace ArchiveViewer::ContextMenus;

TextureFileMenu::TextureFileMenu()
{
	addSeparator();

	const auto ExtractImageFileAction = [this](const char* extension)
	{
		const auto selection = SelectedItems();
		const auto textureFile = selection.at(0).mgfItem;
		const auto fileName = QString(textureFile->FilePath().stem().append(extension).u8string().c_str());

		if (const auto destination = QFileDialog::getSaveFileName(nullptr, QString("Extract %1").arg(fileName), fileName); !destination.isEmpty())
		{
			TextureExtractor.Extract(*textureFile, std::filesystem::path(destination.toLatin1().data()));
		}
	};

	connect(
		addAction("Extract BMP"),
		&QAction::triggered,
		[ExtractImageFileAction]() { ExtractImageFileAction(".bmp"); }
	);

	connect(
		addAction("Extract PNG"),
		&QAction::triggered,
		[ExtractImageFileAction]() { ExtractImageFileAction(".png"); }
	);
}
