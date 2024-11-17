/********************************************************************************
** Form generated from reading UI file 'EntityViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTITYVIEWERWIDGET_H
#define UI_ENTITYVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
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
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QTreeView *entityTreeView;
    QTableView *attributeTableView;

    void setupUi(QWidget *EntityViewerWidget)
    {
        if (EntityViewerWidget->objectName().isEmpty())
            EntityViewerWidget->setObjectName("EntityViewerWidget");
        EntityViewerWidget->resize(1089, 825);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityViewerWidget->sizePolicy().hasHeightForWidth());
        EntityViewerWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(EntityViewerWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        entityTreeView = new QTreeView(EntityViewerWidget);
        entityTreeView->setObjectName("entityTreeView");
        entityTreeView->setMinimumSize(QSize(400, 0));
        entityTreeView->setMaximumSize(QSize(400, 16777215));

        verticalLayout_2->addWidget(entityTreeView);

        attributeTableView = new QTableView(EntityViewerWidget);
        attributeTableView->setObjectName("attributeTableView");
        attributeTableView->setMinimumSize(QSize(400, 0));
        attributeTableView->setMaximumSize(QSize(400, 16777215));

        verticalLayout_2->addWidget(attributeTableView);


        horizontalLayout->addLayout(verticalLayout_2);


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
