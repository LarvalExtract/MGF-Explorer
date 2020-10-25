#ifndef MGFTEXTVIEWWIDGET_H
#define MGFTEXTVIEWWIDGET_H

#include "mgfresourceviewer.h"

#include <QWidget>

namespace Ui {
class MGFTextViewWidget;
}

class MGFTextViewWidget : public QWidget, public AMGFResourceViewer
{
    Q_OBJECT

public:
    explicit MGFTextViewWidget(QWidget *parent = nullptr);
    ~MGFTextViewWidget();

    void LoadMGFItem(const MGFTreeItem *item) override;

private:
    Ui::MGFTextViewWidget *ui;
};

#endif // MGFTEXTVIEWWIDGET_H
