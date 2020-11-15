/********************************************************************************
** Form generated from reading UI file 'listviewtexture.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTVIEWTEXTURE_H
#define UI_LISTVIEWTEXTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListViewTexture
{
public:
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;

    void setupUi(QWidget *ListViewTexture)
    {
        if (ListViewTexture->objectName().isEmpty())
            ListViewTexture->setObjectName(QString::fromUtf8("ListViewTexture"));
        ListViewTexture->resize(857, 562);
        horizontalLayout = new QHBoxLayout(ListViewTexture);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(ListViewTexture);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setShowGrid(false);
        tableView->verticalHeader()->setVisible(false);

        horizontalLayout->addWidget(tableView);


        retranslateUi(ListViewTexture);

        QMetaObject::connectSlotsByName(ListViewTexture);
    } // setupUi

    void retranslateUi(QWidget *ListViewTexture)
    {
        ListViewTexture->setWindowTitle(QCoreApplication::translate("ListViewTexture", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListViewTexture: public Ui_ListViewTexture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTVIEWTEXTURE_H
