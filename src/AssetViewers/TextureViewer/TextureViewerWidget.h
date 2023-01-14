#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/TextureDetailsTableModel.h"

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

        Models::TextureDetailsTable TextureDetailsTableModel;

    private slots:
        void on_ToggleAlphaCheckBox_toggled(bool checked);
        
    protected:
        void showEvent(QShowEvent* event) override;
        void hideEvent(QHideEvent* event) override;
    };

}
