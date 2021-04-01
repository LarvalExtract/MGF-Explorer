#ifndef MGFTEXTUREVIEWERWIDGET_H
#define MGFTEXTUREVIEWERWIDGET_H

#include "AssetViewers/AssetViewerWidgetBase.h"

#include "Models/TextureDetailsTableModel.h"

#include "OgreWindow/ogrewindow.h"

#include <QWidget>
#include <QTableWidgetItem>

class OgreWindow;

namespace Ui {
class TextureViewerWidget;
}

namespace TextureViewer {

    class TextureViewerWidget : public AssetViewerWidgetBase
    {
        Q_OBJECT

    public:
        explicit TextureViewerWidget(QWidget* parent = nullptr);
        ~TextureViewerWidget();

        void LoadAsset(MGF::Asset::AssetPtr asset) override;

    private:
        Ui::TextureViewerWidget* ui;

        Models::TextureDetailsTable TextureDetailsTableModel;

        QWidget* m_Container;
        OgreWindow m_OgreWindow;

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

}

#endif // MGFTEXTUREVIEWERWIDGET_H
