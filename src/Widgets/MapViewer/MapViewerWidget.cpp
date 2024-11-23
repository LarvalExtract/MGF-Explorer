#include "MapViewerWidget.h"
#include "ui_MapViewerWidget.h"

#include "MGF/Assets/MGFMap.h"

MGFMapViewerWidget::MGFMapViewerWidget(QWidget *parent) :
    ui(new Ui::MGFMapViewerWidget)
{
    ui->setupUi(this);
    SceneWidget = new TestWidget(ui->horizontalLayout, this);
}

MGFMapViewerWidget::~MGFMapViewerWidget()
{
    delete ui;
}

void MGFMapViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{
    if (!(sel.isValid() && sel != desel))
    {
        return;
    }

    ui->attributeTableView->setModel(static_cast<WdfEntity*>(sel.internalPointer()));
}

void MGFMapViewerWidget::OnAssetLoaded(MGFAsset& asset)
{
    MGFMap& mapAsset = static_cast<MGFMap&>(asset);

    ui->entityTreeView->setModel(&mapAsset);

    if (mapAsset.TerrainEntity)
    {
        SceneWidget->SetModelEntity(mapAsset.TerrainEntity);
    }

    connect(
        ui->entityTreeView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
        this,
        SLOT(on_EntityTreeView_selectionChanged(const QModelIndex&, const QModelIndex&))
    );
}
