#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/EntityTreeModel.h"

namespace Ui {
    class EntityViewerWidget;
}

namespace EntityViewer {

    class EntityViewerWidget : public IAssetViewerWidget
    {
        Q_OBJECT

    public:
        explicit EntityViewerWidget(QWidget* parent = nullptr);
        ~EntityViewerWidget();

        void LoadAsset(MGF::Asset::AssetPtr asset) override;

    private slots:
        void on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);

    private:
        Ui::EntityViewerWidget* ui;

        Models::EntityTree EntityTreeModel;
    };

}
