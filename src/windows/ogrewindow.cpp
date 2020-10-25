#include "ogrewindow.h"

#include <OgrePlugin.h>

OgreWindow::OgreWindow(QWindow *parent) :
    QWindow(parent),
    m_Root(Ogre::Root::getSingleton())
{  
    installEventFilter(this);
    setFlag(Qt::Widget, true);
    setFlag(Qt::FramelessWindowHint, true);
}

OgreWindow::~OgreWindow()
{
    m_Root.destroyRenderTarget(m_OgreWindow);
}

void OgreWindow::initialize(const QString& windowTitle)
{
    Ogre::NameValuePairList params;
    //params["currentGLContext"] = Ogre::String("false");
    params["externalWindowHandle"] = Ogre::StringConverter::toString(static_cast<size_t>(this->winId()));
    params["border"] = Ogre::String("none");

    //mFSLayer = new Ogre::FileSystemLayer("test");

    try
    {
        m_OgreWindow = m_Root.createRenderWindow(windowTitle.toLatin1().data(), width(), height(), false, &params);
        m_OgreWindow->setVisible(true);

        bInitialised = true;
    }
    catch (Ogre::Exception& e)
    {
        std::string m = e.what();
    }
}

void OgreWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OgreWindow::render()
{
    m_Root.renderOneFrame();
}

void OgreWindow::SetTextureViewer(Ogre::TexturePtr texture)
{

}

void OgreWindow::SetModel(Ogre::SceneNode *model)
{

}

bool OgreWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        render();
        return true;

    default:
        return QWindow::event(event);
    }
}

bool OgreWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (!bInitialised)
        return false;

    if (watched != this)
        return false;

    switch (event->type())
    {
    case QEvent::Resize:
        //if (isExposed())
            resizeEvent(static_cast<QResizeEvent*>(event));
        break;

    case QEvent::Show:
        //if (isExposed())
            //render();
        break;

    case QEvent::Expose:
        render();
        break;
    }

    return false;
}

void OgreWindow::exposeEvent(QExposeEvent *e)
{
    if (isExposed())
        render();
}

void OgreWindow::resizeEvent(QResizeEvent* e)
{
    if (!bInitialised)
        return;

    int w = e->size().width();
    int h = e->size().height();

    m_OgreWindow->resize(w, h);
}

void OgreWindow::InitialiseTextureViewer()
{
    m_TextureViewerScene = m_Root.createSceneManager();
}

void OgreWindow::InitialiseModelViewer()
{
    m_ModelViewerScene = m_Root.createSceneManager();
}
