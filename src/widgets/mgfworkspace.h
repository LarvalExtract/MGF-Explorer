#ifndef MGFWORKSPACE_H
#define MGFWORKSPACE_H

#include "mgf/Archive.h"
#include "widgets/mgftextviewwidget.h"
#include "widgets/mgftextureviewerwidget.h"
#include "widgets/mgmodelviewerwidget.h"
#include "utils/events/EventDispatcher.h"
#include "AssetViewers/StringTableViewer/StringTableViewerWidget.h"
#include "ui_windowfiletable.h"

#include <QWidget>
#include <QString>
#include <QFileIconProvider>
#include <QVector>
#include <QMenu>

namespace Ui {
class MGFWorkspace;
}

class MGFWorkspace : public QWidget, public EventDispatcher
{
    Q_OBJECT

public:
    explicit MGFWorkspace(const QString& mgfFilePath, QWidget *parent = nullptr);
    ~MGFWorkspace();

    inline const MGF::Archive& MGFFile() const { return m_MgfArchive; }
    inline const MGF::File* SelectedItem() const { return m_SelectedItem; }
    QModelIndexList GetSelection() const;

    void ShowFileTableWindow();

private slots:
    void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_customContextMenu_action_extract();

private:
    MGF::Archive m_MgfArchive;
    MGF::File* m_SelectedItem = nullptr;

    Ui::MGFWorkspace *ui;

    QWidget* m_CurrentWidget = nullptr;
    MGFTextViewWidget m_TextWidget;
    MGFTextureViewerWidget m_TextureWidget;
    MGModelViewerWidget m_ModelWidget;
    StringTableViewer::StringTableViewerWidget m_StringTableWidget;

    QWidget m_FileTableWidget;
    Ui_MGFFileTableWindow m_FileTableWindowUi;

    QMenu m_ContextMenu;

private:
    QWidget* DisplayWidget(QWidget *widget);
    void PrintSelectedFileDetails(const MGF::File& selectedFile);
};

#endif // MGFWORKSPACE_H
