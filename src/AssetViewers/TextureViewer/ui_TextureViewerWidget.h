/********************************************************************************
** Form generated from reading UI file 'TextureViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.0.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTUREVIEWERWIDGET_H
#define UI_TEXTUREVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextureViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *TextureDetailsTable;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *TextureViewerWidget)
    {
        if (TextureViewerWidget->objectName().isEmpty())
            TextureViewerWidget->setObjectName(QString::fromUtf8("TextureViewerWidget"));
        TextureViewerWidget->resize(962, 610);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextureViewerWidget->sizePolicy().hasHeightForWidth());
        TextureViewerWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(TextureViewerWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        TextureDetailsTable = new QTableView(TextureViewerWidget);
        TextureDetailsTable->setObjectName(QString::fromUtf8("TextureDetailsTable"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TextureDetailsTable->sizePolicy().hasHeightForWidth());
        TextureDetailsTable->setSizePolicy(sizePolicy1);
        TextureDetailsTable->setMaximumSize(QSize(700, 55));
        TextureDetailsTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(TextureDetailsTable);

        scrollArea = new QScrollArea(TextureViewerWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 958, 546));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


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
