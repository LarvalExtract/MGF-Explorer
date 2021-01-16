/********************************************************************************
** Form generated from reading UI file 'windowfiletable.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWFILETABLE_H
#define UI_WINDOWFILETABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFFileTableWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;

    void setupUi(QWidget *MGFFileTableWindow)
    {
        if (MGFFileTableWindow->objectName().isEmpty())
            MGFFileTableWindow->setObjectName(QString::fromUtf8("MGFFileTableWindow"));
        MGFFileTableWindow->resize(800, 600);
        horizontalLayout = new QHBoxLayout(MGFFileTableWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(MGFFileTableWindow);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(22);

        horizontalLayout->addWidget(tableView);


        retranslateUi(MGFFileTableWindow);

        QMetaObject::connectSlotsByName(MGFFileTableWindow);
    } // setupUi

    void retranslateUi(QWidget *MGFFileTableWindow)
    {
        MGFFileTableWindow->setWindowTitle(QCoreApplication::translate("MGFFileTableWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFFileTableWindow: public Ui_MGFFileTableWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWFILETABLE_H
