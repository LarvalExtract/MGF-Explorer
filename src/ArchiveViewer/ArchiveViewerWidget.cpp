#include "ArchiveViewerWidget.h"
#include "ui_ArchiveViewerWidget.h"

#include "Utilities/ContextProvider/ServiceProvider.h"

#include <QMessageBox>

using namespace ArchiveViewer;

ArchiveViewerWidget::ArchiveViewerWidget(const std::filesystem::path& mgfFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveViewerWidget),
    MgfArchive(mgfFilePath),
    FileTreeModel(MgfArchive.GetFileList()),
    AssetManager(*ServiceProvider::Inject<MGF::AssetManager>())
{
    ui->setupUi(this); 
    ui->Frame->hide();

	ui->AssetViewerStack->addWidget(&PlainTextViewer);
	ui->AssetViewerStack->addWidget(&TextureViewer);
	ui->AssetViewerStack->addWidget(&ModelViewer);
	ui->AssetViewerStack->addWidget(&StringTableViewer);
	ui->AssetViewerStack->addWidget(&EntityViewer);

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
    if (!sel.isValid() || sel == desel)
    {
        return;
    }

    if (const auto& selectedItem = *static_cast<MGF::File*>(sel.internalPointer()); selectedItem.IsFile)
    {
        ui->Frame->show();

		const auto str = QString("%1 | ID: %2 | Offset: %3 | Length: %4 bytes").arg(
            selectedItem.FilePath.c_str(),
			QString::number(selectedItem.GUID, 16),
			QString::number(selectedItem.FileOffset),
			QString::number(selectedItem.FileLength)
		);

        ui->label->setText(str);

        try
        {
			if (const auto asset = this->AssetManager.Get(selectedItem); asset != nullptr)
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

    if (QModelIndex index(ui->FileTreeView->indexAt(pos)); index.isValid())
    {
        if (const auto selectedItem = static_cast<MGF::File*>(index.internalPointer()); selectedItem->IsFile)
        {
			switch (MGF::Asset::AssetBase::ToAssetType(selectedItem->FileType))
			{
            case MGF::Asset::EAssetType::PlainText:     break;
            case MGF::Asset::EAssetType::StringTable:   break;
            case MGF::Asset::EAssetType::Texture:       TextureFileMenu.popup(screenPos); break;
            case MGF::Asset::EAssetType::Model:         break;

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
