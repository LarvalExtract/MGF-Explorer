#include "StringTableViewerWidget.h"
#include "ui_StringTableViewerWidget.h"

#include "MGF/Assets/MGFStringTable.h"

MGFStringTableViewerWidget::MGFStringTableViewerWidget(QWidget *parent) :
    ui(new Ui::MGFStringTableViewerWidget)
{
    ui->setupUi(this);
}

MGFStringTableViewerWidget::~MGFStringTableViewerWidget()
{
    delete ui;
}

void MGFStringTableViewerWidget::OnAssetLoaded(MGFAsset& asset)
{
    MGFStringTable& stringTableAsset = static_cast<MGFStringTable&>(asset);

    ui->tableView->setModel(&stringTableAsset.Strings);
}
