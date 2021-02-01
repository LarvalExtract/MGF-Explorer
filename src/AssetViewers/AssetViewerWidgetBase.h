#pragma once

#include "MGF/Assets/AssetBase.h"

#include <QWidget>

namespace Ui {
    class AssetViewerWidgetBase;
}

class AssetViewerWidgetBase : public QWidget
{
    Q_OBJECT

public:
    explicit AssetViewerWidgetBase(QWidget *parent = nullptr);
    ~AssetViewerWidgetBase();

    virtual void LoadAsset(MGF::Asset::AssetPtr asset);

private slots:


protected:
    Ui::AssetViewerWidgetBase* baseUi;
    MGF::Asset::AssetPtr Asset;
};
