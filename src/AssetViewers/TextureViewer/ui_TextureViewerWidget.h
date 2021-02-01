/********************************************************************************
** Form generated from reading UI file 'TextureViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTUREVIEWERWIDGET_H
#define UI_TEXTUREVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextureViewerWidget
{
public:
    QTableWidget *tableWidget;

    void setupUi(QWidget *TextureViewerWidget)
    {
        if (TextureViewerWidget->objectName().isEmpty())
            TextureViewerWidget->setObjectName(QString::fromUtf8("TextureViewerWidget"));
        TextureViewerWidget->resize(962, 610);
        tableWidget = new QTableWidget(TextureViewerWidget);
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

        retranslateUi(TextureViewerWidget);

        QMetaObject::connectSlotsByName(TextureViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *TextureViewerWidget)
    {
        TextureViewerWidget->setWindowTitle(QCoreApplication::translate("TextureViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextureViewerWidget: public Ui_TextureViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTUREVIEWERWIDGET_H
