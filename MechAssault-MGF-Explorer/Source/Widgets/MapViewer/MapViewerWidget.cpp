#include "MapViewerWidget.h"
#include "ui_MapViewerWidget.h"

#include "MGF/Assets/MGFMap.h"

MGFMapViewerWidget::MGFMapViewerWidget(QWidget *parent) :
    ui(new Ui::MGFMapViewerWidget)
{
    ui->setupUi(this);
    SceneWidget = new TestWidget(ui->MapViewerWidgetContainerLayout, this);
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
	SceneWidget->SetModelEntity(mapAsset.MapRoot);

    QObject::connect(ui->entityTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MGFMapViewerWidget::on_EntityTreeView_selectionChanged);
}
