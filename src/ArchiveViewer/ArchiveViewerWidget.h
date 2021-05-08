#ifndef MGFWORKSPACE_H
#define MGFWORKSPACE_H

#include "MGF/Archive.h"
#include "Models/ArchiveFileTreeModel.h"
#include "AssetViewers/PlainTextViewer/PlainTextViewerWidget.h"
#include "AssetViewers/TextureViewer/TextureViewerWidget.h"
#include "AssetViewers/ModelViewer/ModelViewerWidget.h"
#include "AssetViewers/StringTableViewer/StringTableViewerWidget.h"
#include "AssetViewers/EntityViewer/EntityViewerWidget.h"

#include "ContextMenus/TextureFileMenu.h"

#include "ResourceManager/ResourceManager.h"

namespace Ui {
class ArchiveViewerWidget;
}

namespace ArchiveViewer {

	class ArchiveViewerWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit ArchiveViewerWidget(const QString& mgfFilePath, QWidget* parent = nullptr);
		~ArchiveViewerWidget();

		inline const MGF::Archive& MGFFile() const { return MgfArchive; }

	private slots:
		void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
		void on_treeView_customContextMenuRequested(const QPoint& pos);

	private:
		Ui::ArchiveViewerWidget* ui;

		MGF::Archive MgfArchive;
		Models::ArchiveFileTreeModel FileTreeModel;

		PlainTextViewer::PlainTextViewerWidget PlainTextViewer;
		StringTableViewer::StringTableViewerWidget StringTableViewer;
		TextureViewer::TextureViewerWidget TextureViewer;
		ModelViewer::ModelViewerWidget ModelViewer;
		EntityViewer::EntityViewerWidget EntityViewer;

		ContextMenus::FileMenu FileMenu;
		ContextMenus::TextureFileMenu TextureFileMenu;

	private:
		void DisplayAssetViewer(QWidget* newAssetViewer);
		ResourceManager& AssetManager;
	};

}

#endif // MGFWORKSPACE_H
