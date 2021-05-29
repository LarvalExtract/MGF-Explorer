#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "Utilities/ContextProvider/ServiceProvider.h"

#include <QMessageBox>

using namespace ArchiveViewer;

ArchiveViewerWidget::ArchiveViewerWidget(const QString& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveViewerWidget),
    MgfArchive(mgfFilePath),
    FileTreeModel(MgfArchive.GetFileList()),
    AssetManager(*ServiceProvider::Inject<MGF::AssetManager>())
{
    ui->setupUi(this); 
    ui->Frame->hide();

    ui->FileTreeView->setModel(&FileTreeModel);
    ui->FileTreeView->setColumnWidth(0, 300);
    ui->FileTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        ui->FileTreeView,
        SIGNAL(customContextMenuRequested(QPoint)),
        this,
        SLOT(on_treeView_customContextMenuRequested(const QPoint&))
    );

	connect(
		ui->FileTreeView->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		this,
		SLOT(on_treeView_selectionChanged(const QModelIndex&, const QModelIndex&))
	);

    FileMenu.Initialise(ui->FileTreeView);
    TextureFileMenu.Initialise(ui->FileTreeView);
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

		const auto str = QString("%1 | %2 | %3 | %4 bytes").arg(
            selectedItem.FilePath().c_str(),
			QString::number(selectedItem.GUID(), 16),
			QString::number(selectedItem.FileOffset()),
			QString::number(selectedItem.FileLength())
		);

        ui->label->setText(str);

        try
        {
			if (const auto asset = this->AssetManager.Get(selectedItem); asset != nullptr)
			{
				switch (asset->AssetType)
				{
				case MGF::Asset::EAssetType::PlainText:   DisplayAssetViewer(&PlainTextViewer);   PlainTextViewer.LoadAsset(asset);  break;
				case MGF::Asset::EAssetType::StringTable: DisplayAssetViewer(&StringTableViewer); StringTableViewer.LoadAsset(asset); break;
				case MGF::Asset::EAssetType::Texture:     DisplayAssetViewer(&TextureViewer);     TextureViewer.LoadAsset(asset);  break;
				case MGF::Asset::EAssetType::Model:       DisplayAssetViewer(&ModelViewer);       ModelViewer.LoadAsset(asset); break;
				case MGF::Asset::EAssetType::Entity:      DisplayAssetViewer(&EntityViewer);      EntityViewer.LoadAsset(asset); break;
				}
			}
			else
			{
				DisplayAssetViewer(ui->EmptyWidget);
			}
        }
        catch (const std::exception& e)
        {
            DisplayAssetViewer(ui->EmptyWidget);

            QMessageBox::critical(this, QString("Failed to load asset %1\n%2").arg(selectedItem.Name()), e.what());
        }
    }
    else
    {
        ui->Frame->hide();
    }
}

void ArchiveViewerWidget::DisplayAssetViewer(QWidget* newAssetViewer)
{
	const auto item = ui->AssetViewerLayout->itemAt(2);
	const auto currentAssetViewer = item->widget();
    
    if (currentAssetViewer == newAssetViewer)
    {
        return;
    }

    currentAssetViewer->hide();
    ui->AssetViewerLayout->replaceWidget(currentAssetViewer, newAssetViewer);
    newAssetViewer->show();
}

void ArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    auto screenPos = ui->FileTreeView->viewport()->mapToGlobal(pos);
    if (QModelIndex index(ui->FileTreeView->indexAt(pos)); index.isValid())
    {
        if (const auto selectedItem = static_cast<MGF::File*>(index.internalPointer()); selectedItem->IsFile())
        {
			switch (MGF::Asset::AssetBase::ToAssetType(selectedItem->FileType()))
			{
            case MGF::Asset::EAssetType::PlainText:
            case MGF::Asset::EAssetType::StringTable:
            case MGF::Asset::EAssetType::Texture:       TextureFileMenu.popup(screenPos); break;
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
