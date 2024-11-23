#include "PlainTextViewerWidget.h"
#include "ui_PlainTextViewerWidget.h"

#include "MGF/Assets/MGFPlainText.h"

MGFPlainTextViewerWidget::MGFPlainTextViewerWidget(QWidget *parent) :
    ui(new Ui::MGFPlainTextViewerWidget)
{
    ui->setupUi(this);
}

MGFPlainTextViewerWidget::~MGFPlainTextViewerWidget()
{
    delete ui;
}

void MGFPlainTextViewerWidget::OnAssetLoaded(MGFAsset& asset)
{
    MGFPlainText& plainTextAsset = static_cast<MGFPlainText&>(asset);

    ui->plainTextEdit->setPlainText(plainTextAsset.Text);
}
