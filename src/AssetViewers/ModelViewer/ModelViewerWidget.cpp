#include "ModelViewerWidget.h"
#include "ui_ModelViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/ModelAsset.h"

#include <QMouseEvent>

#include <QRandomGenerator>
#include <QForwardRenderer>
#include <QEntity>
#include <QCamera>
#include <QFirstPersonCameraController>
#include <QCuboidMesh>
#include <QTransform>
#include <QPhongMaterial>

using namespace ModelViewer;

bool ModelViewerWidget::sStaticInitialized = false;
Qt3DCore::QEntity* ModelViewerWidget::sRootEntity = nullptr;
Qt3DRender::QCamera* ModelViewerWidget::sCamera = nullptr;
Qt3DExtras::QFirstPersonCameraController* ModelViewerWidget::sCameraController = nullptr;
Qt3DExtras::QCuboidMesh* ModelViewerWidget::sCube = nullptr;

ModelViewerWidget::ModelViewerWidget(QWidget *parent) :
    ui(new Ui::ModelViewerWidget)
{
    ui->setupUi(this);

    if (!ModelViewerWidget::sStaticInitialized)
        ModelViewerWidget::InitScene();
}

ModelViewerWidget::~ModelViewerWidget()
{
    delete ui;
}

void ModelViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
	auto& model = *static_cast<MGF::Asset::ModelAsset*>(asset.get());



    const int random = static_cast<int>(QRandomGenerator::global()->generate64());
    qApp->GetRenderWindow()->defaultFrameGraph()->setClearColor(QColor::fromRgba(QRgb(random)));
    
	ui->animTableView->setModel(model.GetAnimationTableModel());
	ui->meshTableView->setModel(model.GetMeshTableModel());
	ui->materialTableView->setModel(model.GetMaterialTableModel());
	ui->nodeTreeView->setModel(model.GetNodeTreeModel());
	ui->nodeTreeView->setColumnWidth(0, 400);

    const auto RemoveOrAddTab = [this](QAbstractItemModel* model, QWidget* tab, int index, const QString& label)
    {
        // tab exists
        if (int tabIndex = ui->tabWidget->indexOf(tab); tabIndex > -1)
        {
            if (model->rowCount() == 0)
            {
                ui->tabWidget->removeTab(tabIndex);
            }
        }
        // tab doesn't exist
        else
        {
            if (model->rowCount() > 0)
            {
                ui->tabWidget->insertTab(index, tab, label);
            }
        }
    };

    constexpr int NodeTabIndex = 0;
    constexpr int AnimationTabIndex = 1;
    constexpr int MeshTabIndex = 2;
    constexpr int MaterialTabIndex = 3;

    static const QString NodeTabLabel = "Nodes";
    static const QString AnimationTabLabel = "Animations";
    static const QString MeshTabLabel = "Meshes";
    static const QString MaterialTabLabel = "Materials";

    RemoveOrAddTab(ui->nodeTreeView->model(),      ui->tabNodes,      NodeTabIndex,      NodeTabLabel);
    RemoveOrAddTab(ui->animTableView->model(),     ui->tabAnimations, AnimationTabIndex, AnimationTabLabel);
    RemoveOrAddTab(ui->meshTableView->model(),     ui->tabMeshes,     MeshTabIndex,      MeshTabLabel);
    RemoveOrAddTab(ui->materialTableView->model(), ui->tabMaterials,  MaterialTabIndex,  MaterialTabLabel);
}

void ModelViewerWidget::InitScene()
{
    if (sStaticInitialized)
        return;

    auto wnd = qApp->GetRenderWindow();

    sRootEntity = new Qt3DCore::QEntity();
    sCamera = wnd->camera();
    sCamera->lens()->setPerspectiveProjection(55.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    sCamera->setPosition(QVector3D(0.0f, 0.0f, 20.0f));
    sCamera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    sCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    sCameraController = new Qt3DExtras::QFirstPersonCameraController(sRootEntity);
    sCameraController->setCamera(sCamera);

    sCube = new Qt3DExtras::QCuboidMesh();

    auto mat = new Qt3DExtras::QPhongMaterial();
    mat->setAmbient(QColor::fromRgba(QRgb(0xffff00ff)));

    auto trns = new Qt3DCore::QTransform();
    trns->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    trns->setScale(3.0f);
    trns->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));

    auto cubeEntity = new Qt3DCore::QEntity(sRootEntity);
    cubeEntity->addComponent(sCube);
    cubeEntity->addComponent(mat);
    cubeEntity->addComponent(trns);

    wnd->setRootEntity(sRootEntity);

    sStaticInitialized = true;
}

void ModelViewerWidget::showEvent(QShowEvent* event)
{
    ui->frameLayout->insertWidget(0, qApp->GetRenderWindowContainer());

    qApp->GetRenderWindowContainer()->setMouseTracking(true);
    qApp->GetRenderWindowContainer()->installEventFilter(this);
    m_WindowTimerId = qApp->GetRenderWindowContainer()->startTimer(16);
}

void ModelViewerWidget::hideEvent(QHideEvent* event)
{
    ui->frameLayout->removeWidget(qApp->GetRenderWindowContainer());

    qApp->GetRenderWindowContainer()->setMouseTracking(false);
    qApp->GetRenderWindowContainer()->removeEventFilter(this);
    qApp->GetRenderWindowContainer()->killTimer(m_WindowTimerId);
}

