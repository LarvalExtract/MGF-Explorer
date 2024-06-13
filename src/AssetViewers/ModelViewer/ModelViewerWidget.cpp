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

using namespace ModelViewer;

Qt3DExtras::Qt3DWindow* ModelViewerWidget::RenderWindowPtr = nullptr;
Qt3DRender::QCamera* ModelViewerWidget::Camera = nullptr;
Qt3DCore::QEntity* ModelViewerWidget::SceneRoot = nullptr;
Qt3DCore::QEntity* ModelViewerWidget::SceneLightEntity = nullptr;
Qt3DRender::QPointLight* ModelViewerWidget::Light = nullptr;
Qt3DCore::QTransform* ModelViewerWidget::LightTransform = nullptr;
Qt3DRender::QCamera* ModelViewerWidget::ModelViewerCamera = nullptr;
Qt3DRender::QCameraSelector* ModelViewerWidget::CameraSelector = nullptr;
Qt3DExtras::QAbstractCameraController* ModelViewerWidget::CameraController = nullptr;
Qt3DRender::QLayer* ModelViewerWidget::OpaqueLayer;
Qt3DRender::QLayer* ModelViewerWidget::TransparentLayer;

ModelViewerWidget::ModelViewerWidget(QWidget *parent) :
    ui(new Ui::ModelViewerWidget)
{
    ui->setupUi(this);

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

    if (CurrentModelEntity)
    {
        CurrentModelEntity->setParent(static_cast<Qt3DCore::QNode*>(nullptr));
    }

    const QRect geom = ModelViewerWidget::RenderWindowPtr->geometry();
    ModelViewerWidget::Camera->lens()->setPerspectiveProjection(55.0f, static_cast<float>(geom.width()) / static_cast<float>(geom.height()), 0.1f, 1000.0f);

    model.mRootNode->setParent(ModelViewerWidget::SceneRoot);
    CurrentModelEntity = model.mRootNode;
    
	// ui->animTableView->setModel(model.GetAnimationTableModel());
	// ui->meshTableView->setModel(model.GetMeshTableModel());
	// ui->materialTableView->setModel(model.GetMaterialTableModel());
	// ui->nodeTreeView->setModel(model.GetNodeTreeModel());
	// ui->nodeTreeView->setColumnWidth(0, 400);

    // const auto RemoveOrAddTab = [this](QAbstractItemModel* model, QWidget* tab, int index, const QString& label)
    // {
    //     // tab exists
    //     if (int tabIndex = ui->tabWidget->indexOf(tab); tabIndex > -1)
    //     {
    //         if (model->rowCount() == 0)
    //         {
    //             ui->tabWidget->removeTab(tabIndex);
    //         }
    //     }
    //     // tab doesn't exist
    //     else
    //     {
    //         if (model->rowCount() > 0)
    //         {
    //             ui->tabWidget->insertTab(index, tab, label);
    //         }
    //     }
    // };
    // 
    // constexpr int NodeTabIndex = 0;
    // constexpr int AnimationTabIndex = 1;
    // constexpr int MeshTabIndex = 2;
    // constexpr int MaterialTabIndex = 3;
    // 
    // static const QString NodeTabLabel = "Nodes";
    // static const QString AnimationTabLabel = "Animations";
    // static const QString MeshTabLabel = "Meshes";
    // static const QString MaterialTabLabel = "Materials";
    // 
    // RemoveOrAddTab(ui->nodeTreeView->model(),      ui->tabNodes,      NodeTabIndex,      NodeTabLabel);
    // RemoveOrAddTab(ui->animTableView->model(),     ui->tabAnimations, AnimationTabIndex, AnimationTabLabel);
    // RemoveOrAddTab(ui->meshTableView->model(),     ui->tabMeshes,     MeshTabIndex,      MeshTabLabel);
    // RemoveOrAddTab(ui->materialTableView->model(), ui->tabMaterials,  MaterialTabIndex,  MaterialTabLabel);
}

void ModelViewerWidget::showEvent(QShowEvent* event)
{
  //  ModelViewerWidget::CameraSelector->setCamera(ModelViewerWidget::Camera);
    ModelViewerWidget::RenderWindowPtr->setRootEntity(ModelViewerWidget::SceneRoot);

    ui->frameLayout->insertWidget(0, qApp->GetRenderWindowContainer());

    qApp->GetRenderWindowContainer()->setMouseTracking(true);
    m_WindowTimerId = qApp->GetRenderWindowContainer()->startTimer(16);
}

void ModelViewerWidget::hideEvent(QHideEvent* event)
{
   // ModelViewerWidget::CameraSelector->setCamera(nullptr);
    ModelViewerWidget::RenderWindowPtr->setRootEntity(nullptr);

    if (CurrentModelEntity)
    {
        CurrentModelEntity->setParent(static_cast<Qt3DCore::QNode*>(nullptr));
        CurrentModelEntity = nullptr;
    }

    ui->frameLayout->removeWidget(qApp->GetRenderWindowContainer());

    qApp->GetRenderWindowContainer()->setMouseTracking(false);
    qApp->GetRenderWindowContainer()->killTimer(m_WindowTimerId);
}

bool ModelViewer::ModelViewerWidget::InitialiseScene(Qt3DExtras::Qt3DWindow* renderWindow, Qt3DRender::QCameraSelector* cameraSelector)
{
    ModelViewerWidget::RenderWindowPtr = renderWindow;
    ModelViewerWidget::CameraSelector = cameraSelector;

    ModelViewerWidget::SceneRoot = new Qt3DCore::QEntity;
    ModelViewerWidget::RenderWindowPtr->setRootEntity(ModelViewerWidget::SceneRoot);

    ModelViewerWidget::Camera = new Qt3DRender::QCamera(cameraSelector);
    cameraSelector->setCamera(ModelViewerWidget::Camera);

    Qt3DRender::QLayerFilter* opaqueLayerFilter = new Qt3DRender::QLayerFilter(ModelViewerWidget::Camera);
    OpaqueLayer = new Qt3DRender::QLayer;
    opaqueLayerFilter->addLayer(OpaqueLayer);

    Qt3DRender::QLayerFilter* transparentLayerFilter = new Qt3DRender::QLayerFilter(ModelViewerWidget::Camera);
    TransparentLayer = new Qt3DRender::QLayer;
    transparentLayerFilter->addLayer(TransparentLayer);

    ModelViewerWidget::SceneLightEntity = new Qt3DCore::QEntity(ModelViewerWidget::SceneRoot);
    ModelViewerWidget::Light = new Qt3DRender::QPointLight;
    ModelViewerWidget::Light->setColor(QColor::fromRgbF(1.0f, 1.0f, 1.0f));
    ModelViewerWidget::Light->setIntensity(1.0f);
    ModelViewerWidget::SceneLightEntity->addComponent(ModelViewerWidget::Light);

    ModelViewerWidget::LightTransform = new Qt3DCore::QTransform;
    ModelViewerWidget::SceneLightEntity->addComponent(ModelViewerWidget::LightTransform);

    ModelViewerWidget::CameraController = new Qt3DExtras::QFirstPersonCameraController(ModelViewerWidget::SceneRoot);
    ModelViewerWidget::CameraController->setCamera(ModelViewerWidget::Camera);

    Qt3DRender::QSortPolicy* opaqueSortPolicy = new Qt3DRender::QSortPolicy(ModelViewer::ModelViewerWidget::SceneRoot);
    opaqueSortPolicy->setParent(opaqueLayerFilter);
    opaqueSortPolicy->setSortTypes(QList<Qt3DRender::QSortPolicy::SortType>{ Qt3DRender::QSortPolicy::FrontToBack });
    
    Qt3DRender::QSortPolicy* transparentSortPolicy = new Qt3DRender::QSortPolicy(ModelViewer::ModelViewerWidget::SceneRoot);
    transparentSortPolicy->setParent(transparentLayerFilter);
    transparentSortPolicy->setSortTypes(QList<Qt3DRender::QSortPolicy::SortType>{ Qt3DRender::QSortPolicy::BackToFront });

    return true;
}

void ModelViewerWidget::on_lightPositionXInput_changed(int value)
{
    const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    ModelViewerWidget::LightTransform->setTranslation(QVector3D(value, lightPosition.y(), lightPosition.z()));
}

void ModelViewerWidget::on_lightPositionYInput_changed(int value)
{
    const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    ModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), value, lightPosition.z()));
}

void ModelViewerWidget::on_lightPositionZInput_changed(int value)
{
    const QVector3D lightPosition = ModelViewerWidget::LightTransform->translation();
    ModelViewerWidget::LightTransform->setTranslation(QVector3D(lightPosition.x(), lightPosition.y(), value));
}

