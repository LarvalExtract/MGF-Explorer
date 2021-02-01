#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"
#include "AssetViewers/ui_AssetViewerWidgetBase.h"


using namespace StringTableViewer;

StringTableViewerWidget::StringTableViewerWidget(QWidget *parent) :
    AssetViewerWidgetBase(parent),
    ui(new Ui::StringTableViewerWidget)
{
    ui->setupUi(this);
    baseUi->assetViewerLayout->addWidget(this);
}

StringTableViewerWidget::~StringTableViewerWidget()
{
    delete ui;
}

void StringTableViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    AssetViewerWidgetBase::LoadAsset(asset);
}
