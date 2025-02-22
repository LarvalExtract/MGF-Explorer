#pragma once

#include "Widgets/IAssetViewerWidget.h"
#include "Models/StringTableModel.h"

namespace Ui {
    class MGFStringTableViewerWidget;
}

class MGFStringTableViewerWidget : public IAssetViewerWidget
{
    Q_OBJECT

public:
    explicit MGFStringTableViewerWidget(QWidget* parent = nullptr);
    ~MGFStringTableViewerWidget();

    void OnAssetLoaded(MGFAsset& asset) override;

private:
    Ui::MGFStringTableViewerWidget* ui;

    Models::StringTableModel StringTableModel;
};

