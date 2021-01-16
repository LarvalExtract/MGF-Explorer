/********************************************************************************
** Form generated from reading UI file 'mgftextureviewerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MGFTEXTUREVIEWERWIDGET_H
#define UI_MGFTEXTUREVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFTextureViewerWidget
{
public:
    QTableWidget *tableWidget;

    void setupUi(QWidget *MGFTextureViewerWidget)
    {
        if (MGFTextureViewerWidget->objectName().isEmpty())
            MGFTextureViewerWidget->setObjectName(QString::fromUtf8("MGFTextureViewerWidget"));
        MGFTextureViewerWidget->resize(962, 610);
        tableWidget = new QTableWidget(MGFTextureViewerWidget);
        if (tableWidget->columnCount() < 9)
            tableWidget->setColumnCount(9);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 631, 51));
        tableWidget->setMinimumSize(QSize(600, 0));
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setShowGrid(false);
        tableWidget->setGridStyle(Qt::NoPen);
        tableWidget->setRowCount(1);
        tableWidget->setColumnCount(9);
        tableWidget->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget->horizontalHeader()->setDefaultSectionSize(70);
        tableWidget->verticalHeader()->setVisible(false);

        retranslateUi(MGFTextureViewerWidget);

        QMetaObject::connectSlotsByName(MGFTextureViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFTextureViewerWidget)
    {
        MGFTextureViewerWidget->setWindowTitle(QCoreApplication::translate("MGFTextureViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFTextureViewerWidget: public Ui_MGFTextureViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MGFTEXTUREVIEWERWIDGET_H
