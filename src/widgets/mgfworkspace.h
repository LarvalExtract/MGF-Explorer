#ifndef MGFWORKSPACE_H
#define MGFWORKSPACE_H

#include "mgf/mgfarchive.h"
#include "widgets/mgftextviewwidget.h"
#include "widgets/mgftextureviewerwidget.h"
#include "widgets/mgmodelviewerwidget.h"
#include "windows/listviewtexture.h"
#include "utils/events/EventDispatcher.h"
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

    inline const MGFArchive& MGFFile() const { return m_MgfArchive; }
    inline const MGFTreeItem* SelectedItem() const { return m_SelectedItem; }
    QModelIndexList GetSelection() const;

    void ShowTextureListWindow();
    void ShowFileTableWindow();

private slots:
    void on_treeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_customContextMenu_action_extract();

private:
    MGFArchive m_MgfArchive;
    MGFTreeItem* m_SelectedItem = nullptr;

    Ui::MGFWorkspace *ui;

    QWidget* m_CurrentWidget = nullptr;
    MGFTextViewWidget m_TextWidget;
    MGFTextureViewerWidget m_TextureWidget;
    MGModelViewerWidget m_ModelWidget;

    ListViewTexture m_TextureListWindow;
    QWidget m_FileTableWidget;
    Ui_MGFFileTableWindow m_FileTableWindowUi;

    QMenu m_ContextMenu;

private:
    QWidget* DisplayWidget(QWidget *widget);

};

#endif // MGFWORKSPACE_H
