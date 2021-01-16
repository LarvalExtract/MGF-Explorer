#ifndef MGFTEXTUREVIEWERWIDGET_H
#define MGFTEXTUREVIEWERWIDGET_H

#include "mgfresourceviewer.h"
#include "windows/ogrewindow.h"

#include <QWidget>
#include <QTableWidgetItem>

#include <array>

class OgreWindow;

namespace Ui {
class MGFTextureViewerWidget;
}

class MGFTextureViewerWidget : public QWidget, public AMGFResourceViewer
{
    Q_OBJECT

public:
    explicit MGFTextureViewerWidget(QWidget *parent = nullptr);
    ~MGFTextureViewerWidget();

    void LoadMGFItem(const MGF::File *item);

private:
    Ui::MGFTextureViewerWidget *ui;
    QWidget* m_Container;
    OgreWindow m_OgreWindow;

    std::array<QTableWidgetItem, 9> m_TableItems;

    Ogre::SceneManager* m_SceneManager = nullptr;
    Ogre::Camera* m_OrthoCamera = nullptr;
    Ogre::Viewport* m_TextureViewerViewport = nullptr;
    Ogre::SceneNode* m_OrthoCameraNode = nullptr;
    Ogre::Entity* m_TexturePlane = nullptr;
    Ogre::MaterialPtr m_MatUnlitTextured;
    Ogre::TextureUnitState* m_TextureUnit = nullptr;

private:
    void InitialiseScene();
};

#endif // MGFTEXTUREVIEWERWIDGET_H
