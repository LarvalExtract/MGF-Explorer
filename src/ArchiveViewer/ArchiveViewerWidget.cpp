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
    ui->Frame->hide();

    ui->treeView->setModel(&FileTreeModel);
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        ui->treeView,
        SIGNAL(customContextMenuRequested(QPoint)),
        SLOT(on_treeView_customContextMenuRequested)
    );

	connect(
		ui->treeView->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		this,
		SLOT(on_treeView_selectionChanged(const QModelIndex&, const QModelIndex&))
	);

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

    if (const auto& selectedItem = *static_cast<MGF::File*>(sel.internalPointer()); selectedItem.IsFile())
    {
        ui->Frame->show();

		QString str = QString("%1 | %2 | %3 | %4 bytes").arg(
            selectedItem.FilePath().c_str(),
			QString::number(selectedItem.GUID(), 16),
			QString::number(selectedItem.FileOffset()),
			QString::number(selectedItem.FileLength())
		);

        ui->FileDetails->setText(str);

		if (const auto asset = AssetManager.Get(selectedItem); asset != nullptr)
		{
			switch (asset->GetAssetType())
			{
			case MGF::Asset::EAssetType::PlainText:   DisplayAssetViewer(&PlainTextViewer);   PlainTextViewer.LoadAsset(asset);  break;
            case MGF::Asset::EAssetType::StringTable: DisplayAssetViewer(&StringTableViewer); StringTableViewer.LoadAsset(asset); break;
            case MGF::Asset::EAssetType::Texture:     DisplayAssetViewer(&TextureViewer);     TextureViewer.LoadAsset(asset);  break;
            case MGF::Asset::EAssetType::Model:       DisplayAssetViewer(&ModelViewer);       ModelViewer.LoadAsset(asset); break;
			}
		}
        else
        {
            DisplayAssetViewer(nullptr);
        }
    }
    else
    {
        ui->Frame->hide();


    }
}

void ArchiveViewerWidget::DisplayAssetViewer(QWidget* newAssetViewer)
{
    if (newAssetViewer)
    {
        if (const auto item = ui->AssetViewLayout->itemAt(0); item)
        {
            const auto currentAssetViewer = item->widget();

            if (currentAssetViewer == newAssetViewer)
            {
                return;
            }

            currentAssetViewer->hide();
            ui->AssetViewLayout->replaceWidget(currentAssetViewer, newAssetViewer);
            newAssetViewer->show();
        }
        else
        {
            ui->AssetViewLayout->addWidget(newAssetViewer);
            newAssetViewer->show();
        }
    }
    else
    {
        if (const auto item = ui->AssetViewLayout->itemAt(0); item)
        {
            const auto currentAssetViewer = item->widget();

            currentAssetViewer->hide();
            ui->AssetViewLayout->removeWidget(currentAssetViewer);
        }
    }
}

void ArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    auto screenPos = ui->treeView->viewport()->mapToGlobal(pos);
    if (QModelIndex index(ui->treeView->indexAt(pos)); index.isValid())
    {
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
    else
    {
        FileMenu.popup(screenPos);
    }
}
