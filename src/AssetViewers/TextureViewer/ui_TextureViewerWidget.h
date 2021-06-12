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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextureViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTableView *TextureDetailsTable;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *ToggleAlphaCheckBox;
    QSpacerItem *verticalSpacer;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *TextureViewerWidget)
    {
        if (TextureViewerWidget->objectName().isEmpty())
            TextureViewerWidget->setObjectName(QString::fromUtf8("TextureViewerWidget"));
        TextureViewerWidget->resize(1185, 740);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextureViewerWidget->sizePolicy().hasHeightForWidth());
        TextureViewerWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(TextureViewerWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        TextureDetailsTable = new QTableView(TextureViewerWidget);
        TextureDetailsTable->setObjectName(QString::fromUtf8("TextureDetailsTable"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TextureDetailsTable->sizePolicy().hasHeightForWidth());
        TextureDetailsTable->setSizePolicy(sizePolicy1);
        TextureDetailsTable->setMinimumSize(QSize(700, 55));
        TextureDetailsTable->setMaximumSize(QSize(700, 55));
        TextureDetailsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TextureDetailsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TextureDetailsTable->verticalHeader()->setVisible(false);

        horizontalLayout->addWidget(TextureDetailsTable);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        ToggleAlphaCheckBox = new QCheckBox(TextureViewerWidget);
        ToggleAlphaCheckBox->setObjectName(QString::fromUtf8("ToggleAlphaCheckBox"));

        verticalLayout_2->addWidget(ToggleAlphaCheckBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Ignored);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

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
        ToggleAlphaCheckBox->setText(QCoreApplication::translate("TextureViewerWidget", "Enable Transparency", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextureViewerWidget: public Ui_TextureViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTUREVIEWERWIDGET_H
