/********************************************************************************
** Form generated from reading UI file 'AssetViewerWidgetBase.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSETVIEWERWIDGETBASE_H
#define UI_ASSETVIEWERWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssetViewerWidgetBase
{
public:
    QVBoxLayout *assetViewerLayout;
    QLineEdit *assetFileDetails;

    void setupUi(QWidget *AssetViewerWidgetBase)
    {
        if (AssetViewerWidgetBase->objectName().isEmpty())
            AssetViewerWidgetBase->setObjectName(QString::fromUtf8("AssetViewerWidgetBase"));
        AssetViewerWidgetBase->resize(827, 670);
        assetViewerLayout = new QVBoxLayout(AssetViewerWidgetBase);
        assetViewerLayout->setObjectName(QString::fromUtf8("assetViewerLayout"));
        assetViewerLayout->setContentsMargins(0, 0, 0, 0);
        assetFileDetails = new QLineEdit(AssetViewerWidgetBase);
        assetFileDetails->setObjectName(QString::fromUtf8("assetFileDetails"));
        assetFileDetails->setReadOnly(true);

        assetViewerLayout->addWidget(assetFileDetails);


        retranslateUi(AssetViewerWidgetBase);

        QMetaObject::connectSlotsByName(AssetViewerWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *AssetViewerWidgetBase)
    {
        AssetViewerWidgetBase->setWindowTitle(QCoreApplication::translate("AssetViewerWidgetBase", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AssetViewerWidgetBase: public Ui_AssetViewerWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSETVIEWERWIDGETBASE_H
