#include "TextureFileMenu.h"
#include "MGF/File.h"

#include <QFileDialog>

using namespace ArchiveViewer::ContextMenus;

void TextureFileMenu::Initialise(QTreeView* treeView)
{
	FileMenu::Initialise(treeView);

	addSeparator();

	connect(addAction("Extract BMP"), &QAction::triggered, [this, treeView]()
		{
			const auto& textureFile = *static_cast<MGF::File*>(treeView->selectionModel()->selectedRows(0)[0].internalPointer());
			const auto fileName = QString(textureFile.FilePath().stem().u8string().append(".bmp").c_str());

			if (const auto destination = QFileDialog::getSaveFileName(nullptr, "Extract texture image", fileName); !destination.isEmpty())
			{
				TextureExtractor.Extract(textureFile, std::filesystem::path(destination.toLatin1().data()));
			}
		});

	connect(addAction("Extract PNG"), &QAction::triggered, [this, treeView]()
		{
			const auto& textureFile = *static_cast<MGF::File*>(treeView->selectionModel()->selectedRows(0)[0].internalPointer());
			const auto fileName = QString(textureFile.FilePath().stem().u8string().append(".png").c_str());

			if (const auto destination = QFileDialog::getSaveFileName(nullptr, "Extract texture image", fileName); !destination.isEmpty())
			{
				TextureExtractor.Extract(textureFile, std::filesystem::path(destination.toLatin1().data()));
			}
		});
}
