#include "EntityViewerWidget.h"
#include "ui_EntityViewerWidget.h"

EntityViewerWidget::EntityViewerWidget(QWidget *parent) :
    ui(new Ui::EntityViewerWidget)
{
    ui->setupUi(this);
}

EntityViewerWidget::~EntityViewerWidget()
{
    delete ui;
}

void EntityViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    
}

void EntityViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{

}
