#include "PlainTextViewerWidget.h"
#include "ui_PlainTextViewerWidget.h"
#include "AssetViewers/ui_AssetViewerWidgetBase.h"

#include "MGF/Assets/PlainText.h"

using namespace PlainTextViewer;

PlainTextViewerWidget::PlainTextViewerWidget(QWidget *parent) :
    AssetViewerWidgetBase(parent),
    ui(new Ui::PlainTextViewerWidget)
{
    ui->setupUi(this);
    baseUi->assetViewerLayout->addWidget(ui->plainTextEdit);
}

PlainTextViewerWidget::~PlainTextViewerWidget()
{
    delete ui;
}

void PlainTextViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    AssetViewerWidgetBase::LoadAsset(asset);

    auto plainTextAsset = static_cast<MGF::Asset::PlainText*>(Asset.get());

    ui->plainTextEdit->setPlainText(plainTextAsset->TextData());
}
