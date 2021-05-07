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
    QVBoxLayout *FileTreeLayout;
    QLineEdit *FileDetails;
    QTreeView *FileTreeView;
    QFrame *Frame;
    QVBoxLayout *AssetViewerLayout;

    void setupUi(QWidget *ArchiveViewerWidget)
    {
        if (ArchiveViewerWidget->objectName().isEmpty())
            ArchiveViewerWidget->setObjectName(QString::fromUtf8("ArchiveViewerWidget"));
        ArchiveViewerWidget->resize(1600, 900);
        horizontalLayout = new QHBoxLayout(ArchiveViewerWidget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        FileTreeLayout = new QVBoxLayout();
        FileTreeLayout->setSpacing(5);
        FileTreeLayout->setObjectName(QString::fromUtf8("FileTreeLayout"));
        FileTreeLayout->setSizeConstraint(QLayout::SetMinimumSize);
        FileDetails = new QLineEdit(ArchiveViewerWidget);
        FileDetails->setObjectName(QString::fromUtf8("FileDetails"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileDetails->sizePolicy().hasHeightForWidth());
        FileDetails->setSizePolicy(sizePolicy);
        FileDetails->setMaximumSize(QSize(600, 16777215));

        FileTreeLayout->addWidget(FileDetails);

        FileTreeView = new QTreeView(ArchiveViewerWidget);
        FileTreeView->setObjectName(QString::fromUtf8("FileTreeView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(FileTreeView->sizePolicy().hasHeightForWidth());
        FileTreeView->setSizePolicy(sizePolicy1);
        FileTreeView->setMinimumSize(QSize(550, 0));
        FileTreeView->setMaximumSize(QSize(600, 16777215));
        FileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        FileTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
        FileTreeView->setIconSize(QSize(16, 16));
        FileTreeView->header()->setMinimumSectionSize(42);

        FileTreeLayout->addWidget(FileTreeView);


        horizontalLayout->addLayout(FileTreeLayout);

        Frame = new QFrame(ArchiveViewerWidget);
        Frame->setObjectName(QString::fromUtf8("Frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Frame->sizePolicy().hasHeightForWidth());
        Frame->setSizePolicy(sizePolicy2);
        Frame->setFrameShape(QFrame::StyledPanel);
        Frame->setFrameShadow(QFrame::Raised);
        AssetViewerLayout = new QVBoxLayout(Frame);
        AssetViewerLayout->setSpacing(3);
        AssetViewerLayout->setObjectName(QString::fromUtf8("AssetViewerLayout"));
        AssetViewerLayout->setContentsMargins(0, 0, 0, 0);

        horizontalLayout->addWidget(Frame);


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
