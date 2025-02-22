/********************************************************************************
** Form generated from reading UI file 'StringTableViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRINGTABLEVIEWERWIDGET_H
#define UI_STRINGTABLEVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFStringTableViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;

    void setupUi(QWidget *MGFStringTableViewerWidget)
    {
        if (MGFStringTableViewerWidget->objectName().isEmpty())
            MGFStringTableViewerWidget->setObjectName("MGFStringTableViewerWidget");
        MGFStringTableViewerWidget->resize(600, 500);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MGFStringTableViewerWidget->sizePolicy().hasHeightForWidth());
        MGFStringTableViewerWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MGFStringTableViewerWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(MGFStringTableViewerWidget);
        tableView->setObjectName("tableView");
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(24);

        verticalLayout->addWidget(tableView);


        retranslateUi(MGFStringTableViewerWidget);

        QMetaObject::connectSlotsByName(MGFStringTableViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFStringTableViewerWidget)
    {
        MGFStringTableViewerWidget->setWindowTitle(QCoreApplication::translate("MGFStringTableViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFStringTableViewerWidget: public Ui_MGFStringTableViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRINGTABLEVIEWERWIDGET_H
