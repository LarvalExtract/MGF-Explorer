#pragma once

#include "AssetViewers/AssetViewerWidgetBase.h"

namespace Ui {
    class EntityViewerWidget;
}

class EntityViewerWidget : public AssetViewerWidgetBase
{
    Q_OBJECT

public:
    explicit EntityViewerWidget(QWidget *parent = nullptr);
    ~EntityViewerWidget();

    void LoadAsset(MGF::Asset::AssetPtr asset) override;

private slots:
    void on_EntityTreeView_selectionChanged(const QModelIndex& sel, const QModelIndex& desel);

private:
    Ui::EntityViewerWidget*ui;
};
