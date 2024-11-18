#pragma once

#include "MGF/Archive.h"
#include "MGF/AssetManager.h"

#include "AssetViewers/PlainTextViewer/PlainTextViewerWidget.h"
#include "AssetViewers/TextureViewer/TextureViewerWidget.h"
#include "AssetViewers/ModelViewer/ModelViewerWidget.h"
#include "AssetViewers/StringTableViewer/StringTableViewerWidget.h"
#include "AssetViewers/EntityViewer/EntityViewerWidget.h"

#include "ContextMenus/FolderMenu.h"
#include "ContextMenus/FolderRootMenu.h"
#include "ContextMenus/TextureFileMenu.h"

namespace Ui {
class ArchiveViewerWidget;
}

namespace ArchiveViewer {

	class ArchiveViewerWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit ArchiveViewerWidget(const std::filesystem::path& mgfFilePath, QWidget* parent = nullptr);
		~ArchiveViewerWidget() override;

		void OpenAsset(const std::filesystem::path& assetPath);

		const auto& MGFArchive() const { return MgfArchive; }

	private:
		void OpenAssetViewer(const MGF::File& file);

	private slots:
		void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
		void on_treeView_customContextMenuRequested(const QPoint& pos);

	private:
		Ui::ArchiveViewerWidget* ui;
		MGF::Archive MgfArchive;

		PlainTextViewer::PlainTextViewerWidget PlainTextViewer;
		StringTableViewer::StringTableViewerWidget StringTableViewer;
		TextureViewer::TextureViewerWidget TextureViewer;
		ModelViewer::ModelViewerWidget ModelViewer;
		EntityViewer::EntityViewerWidget EntityViewer;

		ContextMenus::FileMenu FileMenu;
		ContextMenus::FolderMenu FolderMenu;
		ContextMenus::FolderRootMenu FolderRootMenu;
		ContextMenus::TextureFileMenu TextureFileMenu;
	};

}
