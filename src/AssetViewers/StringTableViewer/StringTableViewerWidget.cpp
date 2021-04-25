#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"


using namespace StringTableViewer;

StringTableViewerWidget::StringTableViewerWidget(QWidget *parent) :
    ui(new Ui::StringTableViewerWidget)
{
    ui->setupUi(this);
}

StringTableViewerWidget::~StringTableViewerWidget()
{
    delete ui;
}

void StringTableViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    auto stringTableAsset = static_cast<MGF::Asset::StringTable*>(asset.get());

    StringTableModel.SetAssetReference(stringTableAsset);

    ui->tableView->setModel(&StringTableModel);
}
