#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "Utilities/ContextProvider/ServiceProvider.h"

#include "MGF/Assets/AssetMappings.h"

using namespace ArchiveViewer;

ArchiveViewerWidget::ArchiveViewerWidget(const QString& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveViewerWidget),
    MgfArchive(mgfFilePath),
    FileTreeModel(MgfArchive.GetFileList()),
    AssetManager(*ServiceProvider::Inject<ResourceManager>())
{
    ui->setupUi(this); 

    ui->treeView->setModel(&FileTreeModel);
    ui->treeView->setColumnWidth(0, 300);

    ui->fileDetails->hide();

    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(on_treeView_selectionChanged(const QModelIndex&, const QModelIndex&)));

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_treeView_customContextMenuRequested));

    FileMenu.Initialise(ui->treeView);
}

ArchiveViewerWidget::~ArchiveViewerWidget()
{
    delete ui;
}

void ArchiveViewerWidget::on_treeView_selectionChanged(const QModelIndex &sel, const QModelIndex &desel)
{
    if (!sel.isValid())
        return;

    if (sel == desel)
        return;

    const auto pSelectedItem = static_cast<MGF::File*>(sel.internalPointer());
    const auto asset = AssetManager.Get(*pSelectedItem);

    if (asset)
    {
        switch (asset->GetAssetType())
        {
        case MGF::Asset::EAssetType::PlainText:     pActiveAssetViewer = DisplayAssetViewer(&PlainTextViewer); break;
        case MGF::Asset::EAssetType::StringTable:   pActiveAssetViewer = DisplayAssetViewer(&StringTableViewer); break;
        case MGF::Asset::EAssetType::Texture:       pActiveAssetViewer = DisplayAssetViewer(&TextureViewer); break;
        case MGF::Asset::EAssetType::Model:         pActiveAssetViewer = DisplayAssetViewer(&ModelViewer); break;
        }

        pActiveAssetViewer->LoadAsset(asset);
    }
    else
    {
        pActiveAssetViewer = DisplayAssetViewer(nullptr);
    }
}

AssetViewerWidgetBase* ArchiveViewerWidget::DisplayAssetViewer(AssetViewerWidgetBase* newAssetViewer)
{
    if (pActiveAssetViewer == newAssetViewer)
    {
        return pActiveAssetViewer;
    }

    if (pActiveAssetViewer)
    {
        pActiveAssetViewer->hide();
    }

    if (auto frame = ui->assetViewFrame->layout(); newAssetViewer == nullptr)
    {
        frame->removeWidget(pActiveAssetViewer);
    }
    else
    {
        frame->isEmpty() ? frame->addWidget(newAssetViewer) : (void)frame->replaceWidget(pActiveAssetViewer, newAssetViewer);
    }

    if (newAssetViewer)
    {
        newAssetViewer->show();
    }

    return newAssetViewer;
}

void ArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    if (QModelIndex index(ui->treeView->indexAt(pos)); index.isValid())
    {
        auto screenPos = ui->treeView->viewport()->mapToGlobal(pos);

        if (const auto selectedItem = static_cast<MGF::File*>(index.internalPointer()); selectedItem->IsFile())
        {
			switch (MGF::Asset::sAssetMapping.at(selectedItem->FileType()))
			{
            case MGF::Asset::EAssetType::PlainText:
            case MGF::Asset::EAssetType::StringTable:
            case MGF::Asset::EAssetType::Texture:
            case MGF::Asset::EAssetType::Model:

            default: FileMenu.popup(screenPos); break;
			}
            
        }
        else
        {
            FileMenu.popup(screenPos);
        }
    }
}
