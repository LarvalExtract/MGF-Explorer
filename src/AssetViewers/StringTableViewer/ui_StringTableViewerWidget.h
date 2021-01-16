/********************************************************************************
** Form generated from reading UI file 'StringTableViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
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

class Ui_StringTableViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;

    void setupUi(QWidget *StringTableViewerWidget)
    {
        if (StringTableViewerWidget->objectName().isEmpty())
            StringTableViewerWidget->setObjectName(QString::fromUtf8("StringTableViewerWidget"));
        StringTableViewerWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(StringTableViewerWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, 0, 0);
        tableView = new QTableView(StringTableViewerWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(23);

        verticalLayout->addWidget(tableView);


        retranslateUi(StringTableViewerWidget);

        QMetaObject::connectSlotsByName(StringTableViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *StringTableViewerWidget)
    {
        StringTableViewerWidget->setWindowTitle(QCoreApplication::translate("StringTableViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StringTableViewerWidget: public Ui_StringTableViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRINGTABLEVIEWERWIDGET_H
