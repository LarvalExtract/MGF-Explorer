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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StringTableViewerWidget
{
public:
    QTableView *tableView;

    void setupUi(QWidget *StringTableViewerWidget)
    {
        if (StringTableViewerWidget->objectName().isEmpty())
            StringTableViewerWidget->setObjectName(QString::fromUtf8("StringTableViewerWidget"));
        StringTableViewerWidget->resize(600, 500);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StringTableViewerWidget->sizePolicy().hasHeightForWidth());
        StringTableViewerWidget->setSizePolicy(sizePolicy);
        tableView = new QTableView(StringTableViewerWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(0, 0, 600, 500));
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setDefaultSectionSize(23);

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
