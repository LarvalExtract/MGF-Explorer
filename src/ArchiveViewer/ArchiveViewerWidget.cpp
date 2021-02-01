#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "FileExtractor/FileExtractorDialog.h"

#include "utils/Contexts.h"
#include "ResourceManager/ResourceManager.h"

using namespace ArchiveViewer;

ArchiveViewerWidget::ArchiveViewerWidget(const QString& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveViewerWidget),
    MgfArchive(mgfFilePath),
    FileTreeModel(MgfArchive)
{
    ui->setupUi(this); 

    ui->treeView->setModel(&FileTreeModel);
    ui->treeView->setColumnWidth(0, 300);

    ui->fileDetails->hide();

    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(on_treeView_selectionChanged(const QModelIndex&, const QModelIndex&)));


    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(on_treeView_customContextMenuRequested));

    auto action = m_ContextMenu.addAction("Extract...");
    connect(action, &QAction::triggered, this, &ArchiveViewerWidget::on_customContextMenu_action_extract);
}

ArchiveViewerWidget::~ArchiveViewerWidget()
{
    delete ui;
}

QModelIndexList ArchiveViewerWidget::GetSelection() const
{
    return ui->treeView->selectionModel()->selectedRows();
}

void ArchiveViewerWidget::on_treeView_selectionChanged(const QModelIndex &sel, const QModelIndex &desel)
{
    if (!sel.isValid())
        return;

    if (sel == desel)
        return;

    pSelectedItem = static_cast<MGF::File*>(sel.internalPointer());

    auto rm = Contexts::Get<ResourceManager>();
    auto asset = rm->Get(*pSelectedItem);

    if (asset)
    {
        switch (asset->GetAssetType())
        {
        case MGF::Asset::EAssetType::PlainText:
            pActiveAssetViewer = &PlainTextViewer;
            break;

        case MGF::Asset::EAssetType::StringTable:
            pActiveAssetViewer = &StringTableViewer;
            break;

        case MGF::Asset::EAssetType::Texture:
            pActiveAssetViewer = &TextureViewer;
            break;

        case MGF::Asset::EAssetType::Model:
            pActiveAssetViewer = &ModelViewer;
            break;

        default:
            pActiveAssetViewer = nullptr;
            break;
        }

        pActiveAssetViewer->LoadAsset(asset);
        DisplayWidget(pActiveAssetViewer);
    }
}

QWidget* ArchiveViewerWidget::DisplayWidget(QWidget *widget)
{
    // do nothing
    if (widget == pActiveAssetViewerWidget)
        return widget;

    auto frame = ui->assetViewFrame->layout();

    if (pActiveAssetViewerWidget != nullptr)
        pActiveAssetViewerWidget->hide();

    if (frame->isEmpty())
    {
        // add/remove widget
        if (widget == nullptr)
        {
            return widget;
        }
        else
        {
            frame->addWidget(widget);
            widget->show();
        }
    }
    else
    {
        // change widget
        if (widget == nullptr)
        {
            frame->removeWidget(pActiveAssetViewerWidget);
        }
        else
        {
            frame->replaceWidget(pActiveAssetViewerWidget, widget);
            widget->show();
        }
    }

    return widget;
}

void ArchiveViewerWidget::PrintSelectedFileDetails(const MGF::File& selectedFile)
{
    if (!selectedFile.IsFile())
    {
        ui->fileDetails->hide();
        return;
    }

   
    ui->fileDetails->show();
}

void ArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index(ui->treeView->indexAt(pos));
    if (index.isValid())
    {
        m_ContextMenu.popup(ui->treeView->viewport()->mapToGlobal(pos));
    }
}

void ArchiveViewerWidget::on_customContextMenu_action_extract()
{
	const auto& selection = ui->treeView->selectionModel()->selection().indexes();

    FileExtractor::FileExtractorDialog dialog(this);
    dialog.SetSelection(selection, MgfArchive);
    dialog.exec();
}
