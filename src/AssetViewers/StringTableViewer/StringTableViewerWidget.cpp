#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"


using namespace StringTableViewer;

StringTableViewerWidget::StringTableViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StringTableViewerWidget)
{
    ui->setupUi(this);
}

StringTableViewerWidget::~StringTableViewerWidget()
{
    delete ui;
}

void StringTableViewerWidget::LoadAsset(MGF::AssetPtr asset)
{
    IAssetViewer::LoadAsset(asset);
}
