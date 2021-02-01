#ifndef MGFWORKSPACE_H
#define MGFWORKSPACE_H

#include "MGF/Archive.h"
#include "Models/ArchiveFileTreeModel.h"
#include "AssetViewers/PlainTextViewer/PlainTextViewerWidget.h"
#include "AssetViewers/TextureViewer/TextureViewerWidget.h"
#include "AssetViewers/ModelViewer/ModelViewerWidget.h"
#include "AssetViewers/StringTableViewer/StringTableViewerWidget.h"

#include <QString>
#include <QVector>
#include <QMenu>

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
		inline const MGF::File* SelectedItem() const { return pSelectedItem; }
		QModelIndexList GetSelection() const;

	private slots:
		void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
		void on_treeView_customContextMenuRequested(const QPoint& pos);
		void on_customContextMenu_action_extract();

	private:
		Ui::ArchiveViewerWidget* ui;

		MGF::Archive MgfArchive;
		MGF::File* pSelectedItem = nullptr;

		Models::ArchiveFileTreeModel FileTreeModel;

		AssetViewerWidgetBase* pActiveAssetViewer = nullptr;
		QWidget* pActiveAssetViewerWidget = nullptr;
		PlainTextViewer::PlainTextViewerWidget PlainTextViewer;
		StringTableViewer::StringTableViewerWidget StringTableViewer;
		TextureViewer::TextureViewerWidget TextureViewer;
		ModelViewer::ModelViewerWidget ModelViewer;

		QMenu m_ContextMenu;

	private:
		QWidget* DisplayWidget(QWidget* widget);
		void PrintSelectedFileDetails(const MGF::File& selectedFile);
	};

}

#endif // MGFWORKSPACE_H
