/********************************************************************************
** Form generated from reading UI file 'ArchiveViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARCHIVEVIEWERWIDGET_H
#define UI_ARCHIVEVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArchiveViewerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QVBoxLayout *assetViewerLayout;
    QLineEdit *fileDetails;
    QFrame *assetViewFrame;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *ArchiveViewerWidget)
    {
        if (ArchiveViewerWidget->objectName().isEmpty())
            ArchiveViewerWidget->setObjectName(QString::fromUtf8("ArchiveViewerWidget"));
        ArchiveViewerWidget->resize(1600, 900);
        horizontalLayout = new QHBoxLayout(ArchiveViewerWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        treeView = new QTreeView(ArchiveViewerWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setMaximumSize(QSize(600, 16777215));
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->setSelectionMode(QAbstractItemView::SingleSelection);
        treeView->setIconSize(QSize(16, 16));
        treeView->header()->setMinimumSectionSize(42);

        horizontalLayout->addWidget(treeView);

        assetViewerLayout = new QVBoxLayout();
        assetViewerLayout->setObjectName(QString::fromUtf8("assetViewerLayout"));
        assetViewerLayout->setSizeConstraint(QLayout::SetMinimumSize);
        fileDetails = new QLineEdit(ArchiveViewerWidget);
        fileDetails->setObjectName(QString::fromUtf8("fileDetails"));
        fileDetails->setReadOnly(true);

        assetViewerLayout->addWidget(fileDetails);

        assetViewFrame = new QFrame(ArchiveViewerWidget);
        assetViewFrame->setObjectName(QString::fromUtf8("assetViewFrame"));
        assetViewFrame->setMaximumSize(QSize(16777215, 16777215));
        assetViewFrame->setFrameShape(QFrame::NoFrame);
        assetViewFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(assetViewFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        assetViewerLayout->addWidget(assetViewFrame);


        horizontalLayout->addLayout(assetViewerLayout);


        retranslateUi(ArchiveViewerWidget);

        QMetaObject::connectSlotsByName(ArchiveViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *ArchiveViewerWidget)
    {
        ArchiveViewerWidget->setWindowTitle(QCoreApplication::translate("ArchiveViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArchiveViewerWidget: public Ui_ArchiveViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCHIVEVIEWERWIDGET_H
