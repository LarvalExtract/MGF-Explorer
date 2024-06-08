#include "ModelViewerWidget.h"
#include "ui_ModelViewerWidget.h"

#include "MGFExplorerApplication.h"
#include "MGF/Assets/ModelAsset.h"

#include <QMouseEvent>
#include <QCamera>
#include <QForwardRenderer>


using namespace ModelViewer;

ModelViewerWidget::ModelViewerWidget(QWidget *parent) :
    ui(new Ui::ModelViewerWidget)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    connect(
        ui->lightPositionXInput,
        &QSpinBox::valueChanged,
        this,
        &ModelViewerWidget::on_lightPositionXInput_changed
    );

    connect(
        ui->lightPositionXInput,
        &QSpinBox::valueChanged,
        this,
        &ModelViewerWidget::on_lightPositionYInput_changed
    );

    connect(
        ui->lightPositionXInput,
        &QSpinBox::valueChanged,
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

    auto [wnd, rootEntity] = qApp->GetModelViewerData();
    wnd->defaultFrameGraph()->setClearColor(QColor::fromRgba(0xFFFF00FF));
    wnd->camera()->lens()->setAspectRatio((float)wnd->geometry().width() / (float)wnd->geometry().height());
    wnd->defaultFrameGraph()->setShowDebugOverlay(true);
    model.mRootNode->setParent(rootEntity);
    qApp->mLastEntity = model.mRootNode;
    
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

bool ModelViewer::ModelViewerWidget::eventFilter(QObject* object, QEvent* event)
{
    // fucking stupid default camera controls
    if (event->type() == QEvent::KeyPress)
    {
        using namespace Qt;

        switch (QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event); keyEvent->key())
        {
        case Key_W:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_Up, KeyboardModifier::NoModifier));
            return true;

        case Key_A:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_Left, KeyboardModifier::NoModifier));
            return true;

        case Key_S:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_Down, KeyboardModifier::NoModifier));
            return true;

        case Key_D:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_Right, KeyboardModifier::NoModifier));
            return true;

        case Key_Q:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_PageDown, KeyboardModifier::NoModifier));
            return true;

        case Key_E:
            qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Key_PageUp, KeyboardModifier::NoModifier));
            return true;
        }
    }

    return false;
}

void ModelViewerWidget::on_lightPositionXInput_changed(int value)
{
    qApp->mLightTransform->setTranslation(QVector3D(value, qApp->mLightTransform->translation().y(), qApp->mLightTransform->translation().z()));
}

void ModelViewerWidget::on_lightPositionYInput_changed(int value)
{
    qApp->mLightTransform->setTranslation(QVector3D(qApp->mLightTransform->translation().x(), value, qApp->mLightTransform->translation().z()));
}

void ModelViewerWidget::on_lightPositionZInput_changed(int value)
{
    qApp->mLightTransform->setTranslation(QVector3D(qApp->mLightTransform->translation().x(), qApp->mLightTransform->translation().y(), value));
}

