/********************************************************************************
** Form generated from reading UI file 'mgfworkspace.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MGFWORKSPACE_H
#define UI_MGFWORKSPACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFWorkspace
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout_2;
    QFrame *assetViewFrame;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *MGFWorkspace)
    {
        if (MGFWorkspace->objectName().isEmpty())
            MGFWorkspace->setObjectName(QString::fromUtf8("MGFWorkspace"));
        MGFWorkspace->resize(1600, 900);
        horizontalLayout = new QHBoxLayout(MGFWorkspace);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        treeView = new QTreeView(MGFWorkspace);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setMaximumSize(QSize(600, 16777215));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeView->setIconSize(QSize(16, 16));
        treeView->header()->setMinimumSectionSize(42);

        horizontalLayout->addWidget(treeView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        assetViewFrame = new QFrame(MGFWorkspace);
        assetViewFrame->setObjectName(QString::fromUtf8("assetViewFrame"));
        assetViewFrame->setMaximumSize(QSize(16777215, 16777215));
        assetViewFrame->setFrameShape(QFrame::NoFrame);
        assetViewFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(assetViewFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_2->addWidget(assetViewFrame);


        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(MGFWorkspace);

        QMetaObject::connectSlotsByName(MGFWorkspace);
    } // setupUi

    void retranslateUi(QWidget *MGFWorkspace)
    {
        MGFWorkspace->setWindowTitle(QCoreApplication::translate("MGFWorkspace", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFWorkspace: public Ui_MGFWorkspace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MGFWORKSPACE_H
