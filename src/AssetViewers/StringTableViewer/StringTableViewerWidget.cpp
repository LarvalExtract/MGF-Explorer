#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"
#include "MGF/Assets/StringTableAsset.h"

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
    auto stringTableAsset = static_cast<MGF::Asset::StringTableAsset*>(asset.get());

    ui->tableView->setModel(&stringTableAsset->Strings);
}
