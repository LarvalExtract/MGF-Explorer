#include "MapViewerWidget.h"
#include "ui_MapViewerWidget.h"

#include "MGF/Assets/MGFMap.h"

#include <QEntity>
#include <Qt3DCore/QTransform>

MGFMapViewerWidget::MGFMapViewerWidget(QWidget *parent) :
    ui(new Ui::MGFMapViewerWidget)
{
    ui->setupUi(this);
    SceneWidget = new TestWidget(ui->MapViewerWidgetContainerLayout, this);

    QObject::connect(ui->terrainPositionXInputSpinbox, &QDoubleSpinBox::valueChanged, this, &MGFMapViewerWidget::terrainPositionXInputSpinboxValueChanged);
    QObject::connect(ui->terrainPositionYInputSpinbox, &QDoubleSpinBox::valueChanged, this, &MGFMapViewerWidget::terrainPositionYInputSpinboxValueChanged);
    QObject::connect(ui->terrainPositionZInputSpinbox, &QDoubleSpinBox::valueChanged, this, &MGFMapViewerWidget::terrainPositionZInputSpinboxValueChanged);
    QObject::connect(ui->terrainRotationInputSpinbox, &QDoubleSpinBox::valueChanged, this, &MGFMapViewerWidget::terrainRotationInputSpinboxValueChanged);
    QObject::connect(ui->terrainScaleInputSpinbox, &QDoubleSpinBox::valueChanged, this, &MGFMapViewerWidget::terrainScaleInputSpinboxValueChanged);
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

    if (mapAsset.MapRoot)
    {
        TerrainEntityPtr = mapAsset.TerrainEntity;

        SceneWidget->SetModelEntity(mapAsset.MapRoot);

        Qt3DCore::QTransform* terrainTransform = mapAsset.TerrainEntity->componentsOfType<Qt3DCore::QTransform>()[0];
        ui->terrainPositionXInputSpinbox->setValue(terrainTransform->translation().x());
        ui->terrainPositionYInputSpinbox->setValue(terrainTransform->translation().y());
        ui->terrainPositionZInputSpinbox->setValue(terrainTransform->translation().z());
        ui->terrainRotationInputSpinbox->setValue(terrainTransform->rotationY());
        ui->terrainScaleInputSpinbox->setValue(terrainTransform->scale());
    }

    QObject::connect(ui->entityTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MGFMapViewerWidget::on_EntityTreeView_selectionChanged);
}

void MGFMapViewerWidget::terrainPositionXInputSpinboxValueChanged(double value)
{
    if (TerrainEntityPtr)
    {
        Qt3DCore::QTransform* terrainTransform = TerrainEntityPtr->componentsOfType<Qt3DCore::QTransform>()[0];
        QVector3D terrainPosition = terrainTransform->translation();
        terrainPosition.setX(value);
        terrainTransform->setTranslation(terrainPosition);
    }
}

void MGFMapViewerWidget::terrainPositionYInputSpinboxValueChanged(double value)
{
    if (TerrainEntityPtr)
    {
        Qt3DCore::QTransform* terrainTransform = TerrainEntityPtr->componentsOfType<Qt3DCore::QTransform>()[0];
        QVector3D terrainPosition = terrainTransform->translation();
        terrainPosition.setY(value);
        terrainTransform->setTranslation(terrainPosition);
    }
}

void MGFMapViewerWidget::terrainPositionZInputSpinboxValueChanged(double value)
{
    if (TerrainEntityPtr)
    {
        Qt3DCore::QTransform* terrainTransform = TerrainEntityPtr->componentsOfType<Qt3DCore::QTransform>()[0];
        QVector3D terrainPosition = terrainTransform->translation();
        terrainPosition.setZ(value);
        terrainTransform->setTranslation(terrainPosition);
    }
}

void MGFMapViewerWidget::terrainRotationInputSpinboxValueChanged(double value)
{
    if (TerrainEntityPtr)
    {
        Qt3DCore::QTransform* terrainTransform = TerrainEntityPtr->componentsOfType<Qt3DCore::QTransform>()[0];
        terrainTransform->setRotationY(value);
    }
}

void MGFMapViewerWidget::terrainScaleInputSpinboxValueChanged(double value)
{
    if (TerrainEntityPtr)
    {
        Qt3DCore::QTransform* terrainTransform = TerrainEntityPtr->componentsOfType<Qt3DCore::QTransform>()[0];
        terrainTransform->setScale(value);
    }
}
