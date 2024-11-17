#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/TextureDetailsTableModel.h"

class TestWidget;

namespace Qt3DCore {
    class QEntity;
}

namespace Qt3DExtras {
    class Qt3DWindow;
    class QPlaneMesh;
    class QTextureMaterial;
}

namespace Qt3DRender {
    class QCamera;
}

namespace Ui {
    class TextureViewerWidget;
}

namespace TextureViewer {

    class TextureViewerWidget : public IAssetViewerWidget
    {
        Q_OBJECT

    public:
        explicit TextureViewerWidget(QWidget* parent = nullptr);
        ~TextureViewerWidget() override;

        void LoadAsset(MGF::Asset::AssetPtr asset) override;

    private:
        Ui::TextureViewerWidget* ui;
        TestWidget* SceneWidget = nullptr;

        Models::TextureDetailsTable TextureDetailsTableModel;

        Qt3DRender::QCamera* Camera = nullptr;
        Qt3DCore::QEntity* TextureViewerSceneRoot = nullptr;  
        Qt3DExtras::QTextureMaterial* TextureMaterial = nullptr;
        Qt3DExtras::QPlaneMesh* TextureSurface = nullptr;

    private slots:
        void on_ToggleAlphaCheckBox_toggled(bool checked);
    };

}
