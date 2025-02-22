#pragma once

#include "MGF/MGFArchive.h"

#include "Widgets/PlainTextViewer/PlainTextViewerWidget.h"
#include "Widgets/TextureViewer/TextureViewerWidget.h"
#include "Widgets/ModelViewer/ModelViewerWidget.h"
#include "Widgets/StringTableViewer/StringTableViewerWidget.h"
#include "Widgets/MapViewer/MapViewerWidget.h"

#include "ContextMenus/FolderMenu.h"
#include "ContextMenus/FolderRootMenu.h"
#include "ContextMenus/TextureFileMenu.h"

namespace Ui {
	class MGFArchiveViewerWidget;
}

class MGFArchiveViewerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MGFArchiveViewerWidget(std::shared_ptr<MGFArchive> InMgfArchive, QWidget* parent = nullptr);
	~MGFArchiveViewerWidget() override;

	void OpenAsset(const std::filesystem::path& assetPath);

	const MGFArchive& GetMgfArchive() const { return *MgfArchive.get(); }

private:
	void OpenAssetViewer(const MGFFile& file);

private slots:
	void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
	void on_treeView_customContextMenuRequested(const QPoint& pos);

private:
	Ui::MGFArchiveViewerWidget* ui;

	std::shared_ptr<MGFArchive> MgfArchive;

	MGFModelViewerWidget ModelViewer;
	MGFTextureViewerWidget TextureViewer;
	MGFPlainTextViewerWidget PlainTextViewer;
	MGFStringTableViewerWidget StringTableViewer;
	MGFMapViewerWidget MapViewer;

	ContextMenus::FileMenu FileMenu;
	ContextMenus::FolderMenu FolderMenu;
	ContextMenus::FolderRootMenu FolderRootMenu;
	ContextMenus::TextureFileMenu TextureFileMenu;
};

