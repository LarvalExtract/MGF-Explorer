#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/TextureDetailsTableModel.h"
#include "OgreWindow/ogrewindow.h"

class OgreWindow;

namespace Ui {
class TextureViewerWidget;
}

namespace TextureViewer {

    class TextureViewerWidget : public IAssetViewerWidget
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

    private slots:
        void on_ToggleAlphaCheckBox_toggled(bool checked);
    };

}
