#include "EntityViewerWidget.h"
#include "ui_EntityViewerWidget.h"

#include "MGF/Assets/MapAsset.h"

using namespace EntityViewer;

EntityViewerWidget::EntityViewerWidget(QWidget *parent) :
    ui(new Ui::EntityViewerWidget)
{
    ui->setupUi(this);
    SceneWidget = new TestWidget(ui->horizontalLayout, this);
}

EntityViewerWidget::~EntityViewerWidget()
{
    delete ui;
}

void EntityViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    const auto entityAsset = static_cast<MGF::Asset::WdfMap*>(asset.get());

    ui->entityTreeView->setModel(&entityAsset->GetEntityTreeModel());

    if (entityAsset->TerrainEntity)
    {
        SceneWidget->SetModelEntity(entityAsset->TerrainEntity);
    }

    connect(
        ui->entityTreeView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
        this,
        SLOT(on_EntityTreeView_selectionChanged(const QModelIndex&, const QModelIndex&))
    );
}

void EntityViewerWidget::on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel)
{
    if (!(sel.isValid() && sel != desel))
    {
        return;
    }

    MGF::Asset::WdfEntity& selectedEntity = *static_cast<MGF::Asset::WdfEntity*>(sel.internalPointer());

    ui->attributeTableView->setModel(&selectedEntity);
}
