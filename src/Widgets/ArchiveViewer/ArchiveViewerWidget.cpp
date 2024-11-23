#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "MGF/MGFFile.h"

#include <QMessageBox>

MGFArchiveViewerWidget::MGFArchiveViewerWidget(std::shared_ptr<MGFArchive> InMgfArchive, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MGFArchiveViewerWidget),
    MgfArchive(InMgfArchive),
    PlainTextViewer(this),
    TextureViewer(this),
    ModelViewer(this),
    StringTableViewer(this),
    MapViewer(this)
{
    ui->setupUi(this); 
    ui->Frame->hide();

	ui->AssetViewerStack->addWidget(&PlainTextViewer);
	ui->AssetViewerStack->addWidget(&TextureViewer);
	ui->AssetViewerStack->addWidget(&ModelViewer);
	ui->AssetViewerStack->addWidget(&StringTableViewer);
	ui->AssetViewerStack->addWidget(&MapViewer);

    ui->FileTreeView->setModel(MgfArchive.get());
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

MGFArchiveViewerWidget::~MGFArchiveViewerWidget()
{
    delete ui;
}

void MGFArchiveViewerWidget::OpenAsset(const std::filesystem::path& assetPath)
{
    const MGFFile* assetFile = MgfArchive->Root()->FindRelativeItem(assetPath);

    OpenAssetViewer(*assetFile);
}

void MGFArchiveViewerWidget::on_treeView_selectionChanged(const QModelIndex &sel, const QModelIndex &desel)
{
    if (!sel.isValid() || sel == desel)
    {
        return;
    }

    OpenAssetViewer(*static_cast<const MGFFile*>(sel.internalPointer()));
}

void MGFArchiveViewerWidget::OpenAssetViewer(const MGFFile& file)
{
    if (file.IsFile)
    {
        ui->Frame->show();

        const auto str = QString("%1 | Hash: %2 | Checksum: %3 | Offset: %4 | Length: %5 bytes").arg(
            file.FilePath().c_str(),
            QString::number(file.FilepathHash, 16).toUpper(),
            QString::number(file.FileChecksum, 16).toUpper(),
            QString::number(file.FileOffset),
            QString::number(file.FileLength)
        );

        ui->label->setText(str);

        try
        {
            if (const std::shared_ptr<MGFAsset> asset = MgfArchive->LoadAsset(file); asset != nullptr)
            {
                switch (asset->AssetType)
                {
                case MGFAssetType::Model:       ui->AssetViewerStack->setCurrentWidget(&ModelViewer);       break;
                case MGFAssetType::Texture:     ui->AssetViewerStack->setCurrentWidget(&TextureViewer);     break;
                case MGFAssetType::PlainText:   ui->AssetViewerStack->setCurrentWidget(&PlainTextViewer);   break;
                case MGFAssetType::StringTable: ui->AssetViewerStack->setCurrentWidget(&StringTableViewer); break;
                case MGFAssetType::Map:         ui->AssetViewerStack->setCurrentWidget(&MapViewer);      break;
                }

                static_cast<IAssetViewerWidget*>(ui->AssetViewerStack->currentWidget())->SetAsset(asset);
            }
            else
            {
                ui->AssetViewerStack->setCurrentWidget(ui->EmptyPage);
            }
        }
        catch (const std::exception& e)
        {
            ui->AssetViewerStack->setCurrentWidget(ui->EmptyPage);

            QMessageBox::critical(this, QString("Failed to load asset %1").arg(file.Name), e.what());
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

void MGFArchiveViewerWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    const auto screenPos = ui->FileTreeView->viewport()->mapToGlobal(pos);

    ContextMenus::IContextMenu* menu = nullptr;
    if (const QModelIndex index(ui->FileTreeView->indexAt(pos)); index.isValid())
    {
        if (const MGFFile* selectedItem = static_cast<MGFFile*>(index.internalPointer()); selectedItem && selectedItem->IsFile)
        {
			switch (MGFAsset::MgfFileExtensionToAssetType(selectedItem->FilePath().extension()))
			{
            // case MGF::Asset::EAssetType::PlainText:     break;
            // case MGF::Asset::EAssetType::StringTable:   break;
            case MGFAssetType::Texture:       menu = &TextureFileMenu; break;
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
