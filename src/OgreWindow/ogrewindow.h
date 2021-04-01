#ifndef OGREWINDOW_H
#define OGREWINDOW_H

#include <QWindow>
#include <QResizeEvent>

#include <Ogre.h>

class OgreWindow : public QWindow
{
    Q_OBJECT

public:
    explicit OgreWindow(QWindow* parent = nullptr);
    ~OgreWindow();

    virtual void initialize(const QString& windowTitle);
    virtual void render(QPainter *painter);
    virtual void render();

    Ogre::RenderWindow& GetWindow() { return *m_OgreWindow; }

    void SetTextureViewer(Ogre::TexturePtr texture);
    void SetModel(Ogre::SceneNode* model);

public:
    bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void exposeEvent(QExposeEvent *);
    void resizeEvent(QResizeEvent *);

protected:
    bool bInitialised = false;
    Ogre::Root& m_Root;
    Ogre::RenderWindow* m_OgreWindow = nullptr;

    Ogre::SceneManager* m_CurrentScene = nullptr;

    Ogre::SceneManager* m_ModelViewerScene = nullptr;


    Ogre::SceneManager* m_TextureViewerScene = nullptr;
    Ogre::Viewport* m_TextureViewport = nullptr;
    Ogre::TextureUnitState* m_TextureViewerTU = nullptr;
    Ogre::Camera* m_TextureViewerCamera = nullptr;


private:
    void InitialiseTextureViewer();
    void InitialiseModelViewer();
};

#endif // OGREWINDOW_H
