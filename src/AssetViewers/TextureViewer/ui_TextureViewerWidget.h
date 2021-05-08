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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextureViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QTableView *textureDetailsTable;

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
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(TextureViewerWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        textureDetailsTable = new QTableView(frame);
        textureDetailsTable->setObjectName(QString::fromUtf8("textureDetailsTable"));
        textureDetailsTable->setGeometry(QRect(0, 0, 700, 54));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textureDetailsTable->sizePolicy().hasHeightForWidth());
        textureDetailsTable->setSizePolicy(sizePolicy1);
        textureDetailsTable->setMinimumSize(QSize(700, 0));
        textureDetailsTable->setMaximumSize(QSize(701, 80));
        textureDetailsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textureDetailsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textureDetailsTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(frame);


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
