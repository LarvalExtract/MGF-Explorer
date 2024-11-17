#include "3DSceneWidget.h"
#include "MGFExplorerApplication.h"

#include <QResizeEvent>

#include <QBoxLayout>
#include <QScrollArea>

#include <QDebugOverlay>
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QPlaneMesh>
#include <QTextureMaterial>
#include <QTransform>
#include <QPointLight>
#include <QSortPolicy>
#include <QFilterKey>
#include <QTechniqueFilter>
#include <QClearBuffers>

Scene3dWidget::Scene3dWidget(QWidget* parent)
    : QWidget(parent)
{
    RenderWindow = new Qt3DExtras::Qt3DWindow(nullptr, Qt3DRender::API::OpenGL);
    Container = QWidget::createWindowContainer(RenderWindow, this);
    Container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    RenderSurfaceSelector = new Qt3DRender::QRenderSurfaceSelector;
    RenderSurfaceSelector->setSurface(RenderWindow);

    ClearBuffers = new Qt3DRender::QClearBuffers(RenderSurfaceSelector);
    ClearBuffers->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);
    ClearBuffers->setClearColor(QColorConstants::DarkCyan);

    Viewport = new Qt3DRender::QViewport(RenderSurfaceSelector);

    CameraSelector = new Qt3DRender::QCameraSelector(Viewport);

    TechniqueFilter = new Qt3DRender::QTechniqueFilter(CameraSelector);

    Camera = new Qt3DRender::QCamera(TechniqueFilter);
    CameraSelector->setCamera(Camera);

    OpaqueLayerFilter = new Qt3DRender::QLayerFilter(Camera);
    OpaqueLayerFilter->setFilterMode(Qt3DRender::QLayerFilter::DiscardAnyMatchingLayers);

    OpaqueLayer = new Qt3DRender::QLayer;
    OpaqueLayerFilter->addLayer(OpaqueLayer);

    TransparentLayerFilter = new Qt3DRender::QLayerFilter(Camera);
    TransparentLayerFilter->setFilterMode(Qt3DRender::QLayerFilter::DiscardAnyMatchingLayers);
    TransparentLayer = new Qt3DRender::QLayer;
    TransparentLayerFilter->addLayer(TransparentLayer);

    OpaqueSortPolicy = new Qt3DRender::QSortPolicy(SceneRoot);
    OpaqueSortPolicy->setParent(OpaqueLayerFilter);
    OpaqueSortPolicy->setSortTypes(QList<Qt3DRender::QSortPolicy::SortType>{ Qt3DRender::QSortPolicy::FrontToBack });

    TransparentSortPolicy = new Qt3DRender::QSortPolicy(SceneRoot);
    TransparentSortPolicy->setParent(TransparentLayerFilter);
    TransparentSortPolicy->setSortTypes(QList<Qt3DRender::QSortPolicy::SortType>{ Qt3DRender::QSortPolicy::BackToFront });

    RenderWindow->setActiveFrameGraph(RenderSurfaceSelector);
}

QWidget* Scene3dWidget::GetContainerWidget() const
{
    return Container;
}

Qt3DRender::QCamera* Scene3dWidget::GetCamera() const
{
    return Camera;
}

void Scene3dWidget::SetEntity(Qt3DCore::QEntity* root)
{
    RenderWindow->setRootEntity(SceneRoot = root);
}

void Scene3dWidget::SetCamera(Qt3DRender::QCamera* camera)
{
    CameraSelector->setCamera(camera);
}

Qt3DRender::QLayer* Scene3dWidget::GetTransparentLayer() const
{
    return TransparentLayer;
}

Qt3DRender::QLayer* Scene3dWidget::GetOpaqueLayer() const
{
    return OpaqueLayer;
}

TestWidget::TestWidget(QBoxLayout* frameLayout, QWidget* parent) :
    QWidget(parent),
    RenderWindowContainer(qApp->SceneWidget),
    FrameLayout(frameLayout)
{
    RootEntity = new Qt3DCore::QEntity;
    CameraControllerEntity = new Qt3DExtras::QFirstPersonCameraController(RootEntity);
    CameraControllerEntity->setCamera(RenderWindowContainer->GetCamera());
}

TestWidget::TestWidget(QScrollArea* scrollArea, QWidget* parent) :
    QWidget(parent),
    RenderWindowContainer(qApp->SceneWidget),
    ScrollArea(scrollArea)
{
}

Qt3DRender::QCamera* TestWidget::GetCamera() const
{
    return RenderWindowContainer->GetCamera();
}

void TestWidget::SetModelEntity(Qt3DCore::QEntity* model)
{
    if (model)
    {
        if (ModelEntity)
        {
            ModelEntity->setParent((Qt3DCore::QNode*)nullptr);
        }

        ModelEntity = model;
        ModelEntity->setParent(RootEntity);
    }
}

void TestWidget::SetCamera(Qt3DRender::QCamera* camera)
{
    RenderWindowContainer->SetCamera(camera);
}

void TestWidget::showEvent(QShowEvent* event)
{
    if (FrameLayout)
    {
        FrameLayout->insertWidget(0, RenderWindowContainer->GetContainerWidget());
    }
    else if (ScrollArea)
    {
        ScrollArea->setWidget(RenderWindowContainer->GetContainerWidget());
    }

    RenderWindowContainer->SetEntity(RootEntity);

    RenderWindowContainer->setMouseTracking(true);
    WindowTimerHandle = startTimer(16);
}

void TestWidget::hideEvent(QHideEvent* event)
{
    if (FrameLayout)
    {
        FrameLayout->removeWidget(RenderWindowContainer->GetContainerWidget());
    }
    else if (ScrollArea)
    {
        ScrollArea->setWidget(nullptr);
    }

    RenderWindowContainer->SetEntity(nullptr);

    RenderWindowContainer->setMouseTracking(false);
    killTimer(WindowTimerHandle);
}

void TestWidget::resizeEvent(QResizeEvent* event)
{
    const float aspectRatio = float(event->size().width()) / float(event->size().height());
    GetCamera()->lens()->setPerspectiveProjection(55.0f, aspectRatio, 0.1f, 1000.0f);
}
