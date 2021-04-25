#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "Models/StringTableModel.h"

#include <QWidget>

namespace Ui {
    class StringTableViewerWidget;
}

namespace StringTableViewer {

    class StringTableViewerWidget : public QWidget, public IAssetViewerWidget
    {
        Q_OBJECT

    public:
        explicit StringTableViewerWidget(QWidget* parent = nullptr);
        ~StringTableViewerWidget();

        void LoadAsset(MGF::Asset::AssetPtr asset) override;

    private:
        Ui::StringTableViewerWidget* ui;

        Models::StringTableModel StringTableModel;
    };

}
