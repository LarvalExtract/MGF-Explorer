#include "EntityViewerWidget.h"
#include "ui_EntityViewerWidget.h"
#include "AssetViewers/ui_AssetViewerWidgetBase.h"

EntityViewerWidget::EntityViewerWidget(QWidget *parent) :
    AssetViewerWidgetBase(parent),
    ui(new Ui::EntityViewerWidget)
{
    ui->setupUi(this);
    baseUi->assetViewerLayout->addWidget(ui->Frame);
}

EntityViewerWidget::~EntityViewerWidget()
{
    delete ui;
}

void EntityViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    AssetViewerWidgetBase::LoadAsset(asset);
}

void EntityViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{

}
