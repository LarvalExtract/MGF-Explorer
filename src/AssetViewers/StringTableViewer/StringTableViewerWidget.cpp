#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"
#include "AssetViewers/ui_AssetViewerWidgetBase.h"


using namespace StringTableViewer;

StringTableViewerWidget::StringTableViewerWidget(QWidget *parent) :
    AssetViewerWidgetBase(parent),
    ui(new Ui::StringTableViewerWidget)
{
    ui->setupUi(this);
    baseUi->assetViewerLayout->addWidget(ui->tableView);
}

StringTableViewerWidget::~StringTableViewerWidget()
{
    baseUi->assetViewerLayout->removeWidget(this);
    delete ui;
}

void StringTableViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    AssetViewerWidgetBase::LoadAsset(asset);

    auto stringTableAsset = static_cast<MGF::Asset::StringTable*>(asset.get());

    StringTableModel.SetAssetReference(stringTableAsset);

    ui->tableView->setModel(&StringTableModel);
}
