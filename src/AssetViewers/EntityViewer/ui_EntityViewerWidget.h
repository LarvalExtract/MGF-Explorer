/********************************************************************************
** Form generated from reading UI file 'EntityViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTITYVIEWERWIDGET_H
#define UI_ENTITYVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntityViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *Frame;
    QHBoxLayout *horizontalLayout;
    QTreeView *EntityTreeView;
    QTableView *AttributeTableView;

    void setupUi(QWidget *EntityViewerWidget)
    {
        if (EntityViewerWidget->objectName().isEmpty())
            EntityViewerWidget->setObjectName(QString::fromUtf8("EntityViewerWidget"));
        EntityViewerWidget->resize(1089, 825);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityViewerWidget->sizePolicy().hasHeightForWidth());
        EntityViewerWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(EntityViewerWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Frame = new QFrame(EntityViewerWidget);
        Frame->setObjectName(QString::fromUtf8("Frame"));
        Frame->setFrameShape(QFrame::StyledPanel);
        Frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(Frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        EntityTreeView = new QTreeView(Frame);
        EntityTreeView->setObjectName(QString::fromUtf8("EntityTreeView"));

        horizontalLayout->addWidget(EntityTreeView);

        AttributeTableView = new QTableView(Frame);
        AttributeTableView->setObjectName(QString::fromUtf8("AttributeTableView"));

        horizontalLayout->addWidget(AttributeTableView);


        verticalLayout->addWidget(Frame);


        retranslateUi(EntityViewerWidget);

        QMetaObject::connectSlotsByName(EntityViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *EntityViewerWidget)
    {
        EntityViewerWidget->setWindowTitle(QCoreApplication::translate("EntityViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntityViewerWidget: public Ui_EntityViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTITYVIEWERWIDGET_H
