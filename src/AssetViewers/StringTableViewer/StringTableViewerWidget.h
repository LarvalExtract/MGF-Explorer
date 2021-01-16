#ifndef STRINGTABLEVIEWERWIDGET_H
#define STRINGTABLEVIEWERWIDGET_H

#include "Models/StringTableModel.h"

#include "AssetViewers/IAssetViewer.h"

#include <QWidget>

namespace Ui {
class StringTableViewerWidget;
}

namespace StringTableViewer {

    class StringTableViewerWidget : public QWidget, public IAssetViewer
    {
        Q_OBJECT

    public:
        explicit StringTableViewerWidget(QWidget* parent = nullptr);
        ~StringTableViewerWidget();

        void LoadAsset(MGF::AssetPtr) override;

    private:
        Ui::StringTableViewerWidget* ui;
    };

}

#endif // STRINGTABLEVIEWERWIDGET_H
