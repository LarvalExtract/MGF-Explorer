#include "PlainTextViewerWidget.h"
#include "ui_PlainTextViewerWidget.h"

#include "MGF/Assets/PlainTextAsset.h"

using namespace PlainTextViewer;

PlainTextViewerWidget::PlainTextViewerWidget(QWidget *parent) :
    ui(new Ui::PlainTextViewerWidget)
{
    ui->setupUi(this);
}

PlainTextViewerWidget::~PlainTextViewerWidget()
{
    delete ui;
}

void PlainTextViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    auto plainTextAsset = static_cast<MGF::Asset::PlainTextAsset*>(asset.get());

    ui->plainTextEdit->setPlainText(plainTextAsset->Text);
}
