#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "MGFExplorerApplication.h"

#include <QMessageBox>

using namespace ArchiveViewer;

ArchiveViewerWidget::ArchiveViewerWidget(const std::filesystem::path& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveViewerWidget),
    PlainTextViewer(this),
    TextureViewer(this),
    ModelViewer(this),
    StringTableViewer(this),
    EntityViewer(this),
    MgfArchive{ mgfFilePath }
{
    ui->setupUi(this); 
    ui->Frame->hide();

	ui->AssetViewerStack->addWidget(&PlainTextViewer);
	ui->AssetViewerStack->addWidget(&TextureViewer);
	ui->AssetViewerStack->addWidget(&ModelViewer);
	ui->AssetViewerStack->addWidget(&StringTableViewer);
	ui->AssetViewerStack->addWidget(&EntityViewer);

    ui->FileTreeView->setModel(&MgfArchive);
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

    FileMenu.SelectionModel = ui->FileTreeView->selectionModel();
    FolderMenu.SelectionModel = ui->FileTreeView->selectionModel();
    FolderRootMenu.SelectionModel = ui->FileTreeView->selectionModel();
    TextureFileMenu.SelectionModel = ui->FileTreeView->selectionModel();
}

ArchiveViewerWidget::~ArchiveViewerWidget()
{
    delete ui;
}

void ArchiveViewerWidget::on_treeView_selectionChanged(const QModelIndex &sel, const QModelIndex &desel)
{
    if (!sel.isValid() || sel == desel)
    {
        return;
    }

    if (const auto& selectedItem = *static_cast<MGF::File*>(sel.internalPointer()); selectedItem.IsFile)
    {
        ui->Frame->show();

		const auto str = QString("%1 | Hash: %2 | Checksum: %3 | Offset: %4 | Length: %5 bytes").arg(
            selectedItem.FilePath().c_str(),
			QString::number(selectedItem.FilepathHash, 16).toUpper(),
			QString::number(selectedItem.FileChecksum, 16).toUpper(),
			QString::number(selectedItem.FileOffset),
			QString::number(selectedItem.FileLength)
		);

        ui->label->setText(str);

        try
        {
			if (const auto asset = qApp->AssetManager.Get(selectedItem); asset != nullptr)
			{
				switch (asset->AssetType)
				{
				case MGF::Asset::EAssetType::PlainText:   ui->AssetViewerStack->setCurrentWidget(&PlainTextViewer);   break;
				case MGF::Asset::EAssetType::StringTable: ui->AssetViewerStack->setCurrentWidget(&StringTableViewer); break;
				case MGF::Asset::EAssetType::Texture:     ui->AssetViewerStack->setCurrentWidget(&TextureViewer);     break;
				case MGF::Asset::EAssetType::Model:       ui->AssetViewerStack->setCurrentWidget(&ModelViewer);       break;
				case MGF::Asset::EAssetType::Entity:      ui->AssetViewerStack->setCurrentWidget(&EntityViewer);      break;
				}

                static_cast<IAssetViewerWidget*>(ui->AssetViewerStack->currentWidget())->LoadAsset(asset);
			}
			else
			{
                ui->AssetViewerStack->setCurrentWidget(ui->EmptyPage);
			}
        }
        catch (const std::exception& e)
        {
            ui->AssetViewerStack->setCurrentWidget(ui->EmptyPage);

            QMessageBox::critical(this, QString("Failed to load asset %1").arg(selectedItem.Name), e.what());
#if _DEBUG
            __debugbreak();
#endif
        }
    }
    else
    {
        ui->Frame->hide();
    }
}

void ArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    const auto screenPos = ui->FileTreeView->viewport()->mapToGlobal(pos);

    ContextMenus::IContextMenu* menu = nullptr;
    if (const QModelIndex index(ui->FileTreeView->indexAt(pos)); index.isValid())
    {
        if (const auto selectedItem = static_cast<MGF::File*>(index.internalPointer()); selectedItem->IsFile)
        {
			switch (MGF::Asset::AssetBase::ToAssetType(selectedItem->FileType))
			{
            // case MGF::Asset::EAssetType::PlainText:     break;
            // case MGF::Asset::EAssetType::StringTable:   break;
            case MGF::Asset::EAssetType::Texture:       menu = &TextureFileMenu; break;
            // case MGF::Asset::EAssetType::Model:         break;

            default: menu = &FileMenu; break;
			}
            
        }
        else
        {
			menu = &FolderMenu;
        }
    }
    else
    {
        // root selected
        menu = &FolderRootMenu;
	}
    
    menu->popup(screenPos);
}
