#include "ModelViewerWidget.h"
#include "ui_ModelViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/ModelAsset.h"

#include <QMouseEvent>
#include <QCamera>
#include <QCameraSelector>
#include <QForwardRenderer>
#include <QPointLight>
#include <QDirectionalLight>
#include <QOrbitCameraController>
#include <QFirstPersonCameraController>
#include <QLayerFilter>
#include <QLayer>
#include <QSortPolicy>
#include <QDebugOverlay>
#include <QTechniqueFilter>
#include <QParameter>

using namespace ModelViewer;

ModelViewerWidget::ModelViewerWidget(QWidget *parent) :
    ui(new Ui::ModelViewerWidget)
{
    ui->setupUi(this);

    SceneWidget = new TestWidget(ui->frameLayout, this);

    connect(
        ui->enableSceneLightsCheckBox,
        &QCheckBox::stateChanged,
        this,
        &ModelViewerWidget::on_enableSceneLightsCheckBox_stateChanged
    );

    connect(
        ui->lightPositionSliderXInput,
        &QSlider::valueChanged,
        this,
        &ModelViewerWidget::on_lightPositionXInput_changed
    );

    connect(
        ui->lightPositionSliderYInput,
        &QSlider::valueChanged,
        this,
        &ModelViewerWidget::on_lightPositionYInput_changed
    );

    connect(
        ui->lightPositionSliderZInput,
        &QSlider::valueChanged,
        this,
        &ModelViewerWidget::on_lightPositionZInput_changed
    );
}

ModelViewerWidget::~ModelViewerWidget()
{
    delete ui;
}

void ModelViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
	auto& model = *static_cast<MGF::Asset::ModelAsset*>(asset.get());

    SceneWidget->SetModelEntity(model.mRootNode);

	// ui->animTableView->setModel(model.GetAnimationTableModel());
	// ui->meshTableView->setModel(model.GetMeshTableModel());
	// ui->materialTableView->setModel(model.GetMaterialTableModel());
	ui->nodeTreeView->setModel(&model.Nodes);
	// ui->nodeTreeView->setColumnWidth(0, 400);

   //const auto RemoveOrAddTab = [this](QAbstractItemModel* model, QWidget* tab, int index, const QString& label)
   //{
   //    // tab exists
   //    if (int tabIndex = ui->tabWidget->indexOf(tab); tabIndex > -1)
   //    {
   //        if (model->rowCount() == 0)
   //        {
   //            ui->tabWidget->removeTab(tabIndex);
   //        }
   //    }
   //    // tab doesn't exist
   //    else
   //    {
   //        if (model->rowCount() > 0)
   //        {
   //            ui->tabWidget->insertTab(index, tab, label);
   //        }
   //    }
   //};
   //
   //constexpr int NodeTabIndex = 0;
   //constexpr int AnimationTabIndex = 1;
   //constexpr int MeshTabIndex = 2;
   //constexpr int MaterialTabIndex = 3;
   //
   //static const QString NodeTabLabel = "Nodes";
   //static const QString AnimationTabLabel = "Animations";
   //static const QString MeshTabLabel = "Meshes";
   //static const QString MaterialTabLabel = "Materials";
   //
   //RemoveOrAddTab(ui->nodeTreeView->model(),      ui->tabNodes,      NodeTabIndex,      NodeTabLabel);
   //RemoveOrAddTab(ui->animTableView->model(),     ui->tabAnimations, AnimationTabIndex, AnimationTabLabel);
   //RemoveOrAddTab(ui->meshTableView->model(),     ui->tabMeshes,     MeshTabIndex,      MeshTabLabel);
   //RemoveOrAddTab(ui->materialTableView->model(), ui->tabMaterials,  MaterialTabIndex,  MaterialTabLabel);
}

void ModelViewerWidget::on_lightPositionXInput_changed(int value)
{
    //const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    //ModelViewerWidget::LightTransform->setTranslation(QVector3D(value, lightPosition.y(), lightPosition.z()));
}

void ModelViewerWidget::on_lightPositionYInput_changed(int value)
{
    //const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    //ModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), value, lightPosition.z()));
}

void ModelViewerWidget::on_lightPositionZInput_changed(int value)
{
    //const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    //ModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), lightPosition.y(), value));
}

void ModelViewerWidget::on_enableSceneLightsCheckBox_stateChanged(int state)
{
    //ModelViewerWidget::SceneEnableLights->setValue(state == Qt::Unchecked);
}

