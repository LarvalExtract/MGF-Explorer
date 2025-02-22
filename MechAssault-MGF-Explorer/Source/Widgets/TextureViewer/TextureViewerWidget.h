#pragma once

#include "Widgets/IAssetViewerWidget.h"
#include "Models/TextureDetailsTableModel.h"

#include "Qt3DForwardDeclarations.h"

class TestWidget;

namespace Ui {
    class MGFTextureViewerWidget;
}

class MGFTextureViewerWidget : public IAssetViewerWidget
{
    Q_OBJECT

public:
    explicit MGFTextureViewerWidget(QWidget* parent = nullptr);
    ~MGFTextureViewerWidget() override;

    void OnAssetLoaded(MGFAsset& asset) override;

private:
    Ui::MGFTextureViewerWidget* ui;
    TestWidget* SceneWidget = nullptr;

    Models::TextureDetailsTable TextureDetailsTableModel;

    Qt3DRender::QCamera* Camera = nullptr;
    Qt3DCore::QEntity* TextureViewerSceneRoot = nullptr;  
    Qt3DExtras::QTextureMaterial* TextureMaterial = nullptr;
    Qt3DExtras::QPlaneMesh* TextureSurface = nullptr;

private slots:
    void on_ToggleAlphaCheckBox_toggled(bool checked);
};

