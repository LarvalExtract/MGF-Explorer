#pragma once

#include "Widgets/IAssetViewerWidget.h"
#include "Widgets/3DSceneWidget.h"

namespace Ui {
    class MGFMapViewerWidget;
}

class MGFMapViewerWidget : public IAssetViewerWidget
{
    Q_OBJECT

public:
    explicit MGFMapViewerWidget(QWidget* parent = nullptr);
    ~MGFMapViewerWidget();

    void OnAssetLoaded(MGFAsset& asset) override;

private slots:
    void on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);

private:
    Ui::MGFMapViewerWidget* ui;
    TestWidget* SceneWidget;
};

