#include "ModelViewerWidget.h"
#include "ui_ModelViewerWidget.h"
#include "AssetViewers/ui_AssetViewerWidgetBase.h"

#include "MGF/Assets/ModelAsset.h"

static int s_Count = 0;

using namespace ModelViewer;

ModelViewerWidget::ModelViewerWidget(QWidget *parent) :
    AssetViewerWidgetBase(parent),
    ui(new Ui::ModelViewerWidget)
{
    ui->setupUi(this);
    baseUi->assetViewerLayout->addWidget(this);

    s_Count++;

    QString title("ModelViewerWindow");
    title += QString::number(s_Count);
    m_OgreWindow.initialize(title);

    m_OgreWindowContainer = QWidget::createWindowContainer(&m_OgreWindow, this);
    static_cast<QVBoxLayout*>(layout())->insertWidget(0, m_OgreWindowContainer);

    InitialiseScene();

    m_OgreWindowContainer->setMouseTracking(true);
    m_OgreWindowContainer->installEventFilter(this);
    m_OgreWindow.installEventFilter(this);
    m_OgreWindowContainer->startTimer(16);

    ui->tabWidget->clear();   
}

ModelViewerWidget::~ModelViewerWidget()
{
    m_SceneManager->clearScene();
    Ogre::Root::getSingleton().destroySceneManager(m_SceneManager);

    delete ui;
}

void ModelViewerWidget::LoadAsset(MGF::Asset::AssetPtr asset)
{
    AssetViewerWidgetBase::LoadAsset(asset);

	m_SceneManager->getRootSceneNode()->removeAllChildren();
	auto& model = static_cast<MGF::Asset::ModelAsset&>(*(asset.get()));
	m_SceneManager->getRootSceneNode()->addChild(model.GetRootNode());

    MeshTableModel.SetAssetReference(&model);
    MaterialTableModel.SetAssetReference(&model);

	//ui->animTableView->setModel(model.GetAnimationTableModel());
	ui->meshTableView->setModel(&MeshTableModel);
	ui->materialTableView->setModel(&MaterialTableModel);
	//ui->nodeTreeView->setModel(model.GetNodeTreeModel());

	//UpdateTabs();

	ui->nodeTreeView->setColumnWidth(0, 400);

	// Reset camera position
	model.GetRootNode()->_update(true, true);
	const auto& aabb = model.GetRootNode()->_getWorldAABB();

	ResetCamera(aabb);
}

void ModelViewerWidget::InitialiseScene()
{
    m_SceneManager = Ogre::Root::getSingleton().createSceneManager();

    m_Camera = m_SceneManager->createCamera("ModelViewerCamera");
    m_Camera->setNearClipDistance(0.1f);
    m_Camera->setFarClipDistance(10000.0f);
    m_Camera->setAutoAspectRatio(true);

    m_CameraNode = m_SceneManager->getRootSceneNode()->createChildSceneNode();
    m_CameraNode->attachObject(m_Camera);
    m_CameraNode->setPosition(0.0f, 0.0f, -100.0f);

    m_Viewport = m_OgreWindow.GetWindow().addViewport(m_Camera);
    m_Viewport->setBackgroundColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

    m_SceneManager->setAmbientLight(Ogre::ColourValue());

    m_SceneRoot = m_SceneManager->getRootSceneNode();
}

// handle events fired by the Ogre window container
// be sure to return false - returning true swallows the event
bool ModelViewerWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (!isVisible())
        return false;

    auto OnButtonPress = [&event, this]()
    {
        Qt::MouseButton btn = static_cast<QMouseEvent*>(event)->button();
        if (btn == Qt::MouseButton::LeftButton)
        {
            m_bMouseHeld = true;
            auto c = this->cursor();
            m_CursorLocation = c.pos();
            c.setShape(Qt::CursorShape::BlankCursor);
            this->setCursor(c);
        }
    };

    auto OnButtonRelease = [&event, this]()
    {
        Qt::MouseButton btn = static_cast<QMouseEvent*>(event)->button();
        if (btn == Qt::MouseButton::LeftButton)
        {
            m_bMouseHeld = false;
            auto c = this->cursor();
            c.setShape(Qt::CursorShape::ArrowCursor);
            this->setCursor(c);
        }
    };

    if (watched == m_OgreWindowContainer)
    {
        if (event->type() == QEvent::Timer)
        {
            UpdateFrame(static_cast<QTimerEvent*>(event));
            return true;
        }
    }     

    else if (watched == &m_OgreWindow)
    {
        auto c = cursor();

        switch (event->type())
        {
        case QEvent::MouseButtonPress:
            OnButtonPress();
            break;

        case QEvent::MouseButtonRelease:
            OnButtonRelease();
            break;

        case QEvent::MouseMove:
            if (m_bMouseHeld)
                UpdateCamera();
            break;

        case QEvent::Wheel:
            if (m_bMouseHeld)
            {
                m_ScrollAngle += (float)static_cast<QWheelEvent*>(event)->angleDelta().y() / 120.0f;
                m_ScrollAngle = Ogre::Math::Clamp(m_ScrollAngle, 0.0f, 10.0f);
                m_CameraSpeed = (0.1f * (m_ScrollAngle * m_ScrollAngle)) + 0.01f;
            }
            break;

        case QEvent::KeyPress:
            UpdateKeyState(static_cast<QKeyEvent*>(event)->key(), true);
            break;

        case QEvent::KeyRelease:
            UpdateKeyState(static_cast<QKeyEvent*>(event)->key(), false);
            break;
        }

        return true;
    }

    return QWidget::eventFilter(watched, event);
}

void ModelViewerWidget::UpdateFrame(QTimerEvent* timerEvt)
{
    if (m_bMouseHeld)
    {
        Ogre::Vector3 move = m_CameraNode->getPosition();
        Ogre::Vector3 deltaF = m_Forward * m_CameraSpeed;
        Ogre::Vector3 deltaR = m_Right * m_CameraSpeed;
        Ogre::Vector3 deltaU = -m_Up * m_CameraSpeed;

        if (m_bIsWPressed)
            move += deltaF;
        if (m_bIsAPressed)
            move -= deltaR;
        if (m_bIsSPressed)
            move -= deltaF;
        if (m_bIsDPressed)
            move += deltaR;
        if (m_bIsSpacePressed)
            move += deltaU;
        if (m_bIsCtrlPressed)
            move -= deltaU;

        m_CameraNode->setPosition(move);
    }

    m_OgreWindow.render();
}

void ModelViewerWidget::ResetCamera(const Ogre::AxisAlignedBox &modelAABB)
{
    Ogre::Vector3 size(1.0f);
    Ogre::Vector3 center(0.0f);

    if (!modelAABB.isNull())
    {
        size = modelAABB.getSize();
        center = modelAABB.getCenter();
    }

    float distance = size.length();
    distance *= 1.10f;

    Ogre::Vector3 pos = center;
    pos.x += Ogre::Math::Cos(Ogre::Degree(45.0f)) * distance;
    pos.y += distance / 4.0f;
    pos.z += Ogre::Math::Sin(Ogre::Degree(45.0f)) * distance;

    Ogre::Vector3 dir = center - pos;
    dir.normalise();

    m_CameraNode->setPosition(pos);
    m_Camera->setDirection(dir);

    Ogre::Vector2 adjacent(dir.x, dir.z);
    m_CameraYaw = Ogre::Math::ATan2(adjacent.y, adjacent.x).valueRadians();
    m_CameraPitch = Ogre::Math::ATan2(dir.y, adjacent.length()).valueRadians();
}

void ModelViewerWidget::UpdateCamera()
{
    auto c = cursor();
    QPoint currentPos = c.pos();
    QPoint delta = currentPos - m_CursorLocation;

    float yaw = -Ogre::Math::DegreesToRadians(static_cast<float>(delta.x()) * 0.1f);
    float pitch = -Ogre::Math::DegreesToRadians(static_cast<float>(delta.y()) * 0.1f);
    m_CameraYaw += yaw;
    m_CameraPitch += pitch;

    m_CameraYaw = std::fmodf(m_CameraYaw, Ogre::Math::TWO_PI);
    m_CameraPitch = Ogre::Math::Clamp<float>(m_CameraPitch, -Ogre::Math::HALF_PI, Ogre::Math::HALF_PI);

    UpdateCameraAxes(m_CameraYaw, m_CameraPitch);

    m_Camera->setDirection(m_Forward);

    c.setPos(m_CursorLocation);
    setCursor(c);
}

void ModelViewerWidget::UpdateCameraAxes(float yaw, float pitch)
{
    m_Forward.x = Ogre::Math::Cos(pitch) * Ogre::Math::Sin(yaw);
    m_Forward.y = Ogre::Math::Sin(pitch);
    m_Forward.z = Ogre::Math::Cos(pitch) * Ogre::Math::Cos(yaw);

    m_Right.x = Ogre::Math::Sin(yaw - Ogre::Math::HALF_PI);
    m_Right.y = 0.0f;
    m_Right.z = Ogre::Math::Cos(yaw - Ogre::Math::HALF_PI);

    m_Up = -m_Right.crossProduct(m_Forward);
}

void ModelViewerWidget::UpdateKeyState(int key, bool state)
{
    switch (key)
    {
    case Qt::Key::Key_W: m_bIsWPressed = state; break;
    case Qt::Key::Key_A: m_bIsAPressed = state; break;
    case Qt::Key::Key_S: m_bIsSPressed = state; break;
    case Qt::Key::Key_D: m_bIsDPressed = state; break;
    case Qt::Key::Key_Space: m_bIsSpacePressed = state; break;
    case Qt::Key::Key_Control: m_bIsCtrlPressed = state; break;
    }
}

void ModelViewerWidget::UpdateTabs()
{
    const auto setTab = [this](QAbstractItemView* modelView, const QString& title)
    {
        if (modelView->model()->rowCount() == 0)
            ui->tabWidget->removeTab(ui->tabWidget->indexOf(modelView));
        else
            ui->tabWidget->addTab(modelView, title);
    };

    // Add node tab
    setTab(ui->nodeTreeView, "Nodes");
    setTab(ui->animTableView, "Animations");
    setTab(ui->meshTableView, "Meshes");
    setTab(ui->materialTableView, "Materials");
}

