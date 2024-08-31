#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/TextureDetailsTableModel.h"

namespace Qt3DCore {
    class QEntity;
}

namespace Qt3DExtras {
    class Qt3DWindow;
    class QPlaneMesh;
    class QTextureMaterial;
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

        static bool InitialiseScene(Qt3DExtras::Qt3DWindow* renderWindow);

    private:
        Ui::TextureViewerWidget* ui;

        Models::TextureDetailsTable TextureDetailsTableModel;

        static Qt3DExtras::Qt3DWindow* RenderWindowPtr;
        static Qt3DCore::QEntity* TextureViewerSceneRoot;
        static Qt3DExtras::QTextureMaterial* TextureMaterial;
        static Qt3DExtras::QPlaneMesh* TextureSurface;

    private slots:
        void on_ToggleAlphaCheckBox_toggled(bool checked);
        
    protected:
        void showEvent(QShowEvent* event) override;
        void hideEvent(QHideEvent* event) override;
    };

}
