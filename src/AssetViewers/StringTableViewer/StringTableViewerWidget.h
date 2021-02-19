#ifndef STRINGTABLEVIEWERWIDGET_H
#define STRINGTABLEVIEWERWIDGET_H

#include "AssetViewers/AssetViewerWidgetBase.h"
#include "Models/StringTableModel.h"

namespace Ui {
class StringTableViewerWidget;
}

namespace StringTableViewer {

    class StringTableViewerWidget : public AssetViewerWidgetBase
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

#endif // STRINGTABLEVIEWERWIDGET_H
