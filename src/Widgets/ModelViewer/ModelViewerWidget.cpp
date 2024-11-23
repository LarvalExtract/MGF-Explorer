#include "ModelViewerWidget.h"
#include "ui_ModelViewerWidget.h"

#include "MGF/Assets/MGFModel.h"

MGFModelViewerWidget::MGFModelViewerWidget(QWidget *parent) :
    ui(new Ui::MGFModelViewerWidget)
{
    ui->setupUi(this);

    SceneWidget = new TestWidget(ui->frameLayout, this);

    connect(
        ui->enableSceneLightsCheckBox,
        &QCheckBox::stateChanged,
        this,
        &MGFModelViewerWidget::on_enableSceneLightsCheckBox_stateChanged
    );

    connect(
        ui->lightPositionSliderXInput,
        &QSlider::valueChanged,
        this,
        &MGFModelViewerWidget::on_lightPositionXInput_changed
    );

    connect(
        ui->lightPositionSliderYInput,
        &QSlider::valueChanged,
        this,
        &MGFModelViewerWidget::on_lightPositionYInput_changed
    );

    connect(
        ui->lightPositionSliderZInput,
        &QSlider::valueChanged,
        this,
        &MGFModelViewerWidget::on_lightPositionZInput_changed
    );
}

MGFModelViewerWidget::~MGFModelViewerWidget()
{
    delete ui;
}

void MGFModelViewerWidget::OnAssetLoaded(MGFAsset& asset)
{
	MGFModel& model = static_cast<MGFModel&>(asset);

    SceneWidget->SetModelEntity(model.mRootNode);

	// ui->animTableView->setModel(model.GetAnimationTableModel());
	// ui->meshTableView->setModel(model.GetMeshTableModel());
	// ui->materialTableView->setModel(model.GetMaterialTableModel());
	//ui->nodeTreeView->setModel(&model.Nodes);
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

void MGFModelViewerWidget::on_lightPositionXInput_changed(int value)
{
    //const QVector3D lightPosition = MGFModelViewerWidget::LightTransform->translation();
    //MGFModelViewerWidget::LightTransform->setTranslation(QVector3D(value, lightPosition.y(), lightPosition.z()));
}

void MGFModelViewerWidget::on_lightPositionYInput_changed(int value)
{
    //const QVector3D lightPosition = MGFModelViewerWidget::LightTransform->translation();
    //MGFModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), value, lightPosition.z()));
}

void MGFModelViewerWidget::on_lightPositionZInput_changed(int value)
{
    //const QVector3D lightPosition = MGFModelViewerWidget::LightTransform->translation();
    //MGFModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), lightPosition.y(), value));
}

void MGFModelViewerWidget::on_enableSceneLightsCheckBox_stateChanged(int state)
{
    //MGFModelViewerWidget::SceneEnableLights->setValue(state == Qt::Unchecked);
}

