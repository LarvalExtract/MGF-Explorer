/********************************************************************************
** Form generated from reading UI file 'ArchiveViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArchiveViewerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *FileTreeLayout;
    QTreeView *FileTreeView;
    QFrame *Frame;
    QVBoxLayout *AssetViewerLayout;
    QLabel *label;
    QStackedWidget *AssetViewerStack;
    QWidget *EmptyPage;

    void setupUi(QWidget *ArchiveViewerWidget)
    {
        if (ArchiveViewerWidget->objectName().isEmpty())
            ArchiveViewerWidget->setObjectName("ArchiveViewerWidget");
        ArchiveViewerWidget->resize(1600, 900);
        horizontalLayout = new QHBoxLayout(ArchiveViewerWidget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        FileTreeLayout = new QVBoxLayout();
        FileTreeLayout->setSpacing(5);
        FileTreeLayout->setObjectName("FileTreeLayout");
        FileTreeLayout->setSizeConstraint(QLayout::SetMinimumSize);
        FileTreeView = new QTreeView(ArchiveViewerWidget);
        FileTreeView->setObjectName("FileTreeView");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileTreeView->sizePolicy().hasHeightForWidth());
        FileTreeView->setSizePolicy(sizePolicy);
        FileTreeView->setMinimumSize(QSize(600, 0));
        FileTreeView->setMaximumSize(QSize(600, 16777215));
        FileTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        FileTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
        FileTreeView->setIconSize(QSize(16, 16));
        FileTreeView->header()->setMinimumSectionSize(42);

        FileTreeLayout->addWidget(FileTreeView);


        horizontalLayout->addLayout(FileTreeLayout);

        Frame = new QFrame(ArchiveViewerWidget);
        Frame->setObjectName("Frame");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Frame->sizePolicy().hasHeightForWidth());
        Frame->setSizePolicy(sizePolicy1);
        Frame->setFrameShape(QFrame::StyledPanel);
        Frame->setFrameShadow(QFrame::Raised);
        AssetViewerLayout = new QVBoxLayout(Frame);
        AssetViewerLayout->setSpacing(3);
        AssetViewerLayout->setObjectName("AssetViewerLayout");
        AssetViewerLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(Frame);
        label->setObjectName("label");
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        AssetViewerLayout->addWidget(label);

        AssetViewerStack = new QStackedWidget(Frame);
        AssetViewerStack->setObjectName("AssetViewerStack");
        AssetViewerStack->setFrameShape(QFrame::NoFrame);
        AssetViewerStack->setFrameShadow(QFrame::Sunken);
        EmptyPage = new QWidget();
        EmptyPage->setObjectName("EmptyPage");
        AssetViewerStack->addWidget(EmptyPage);

        AssetViewerLayout->addWidget(AssetViewerStack);


        horizontalLayout->addWidget(Frame);


        retranslateUi(ArchiveViewerWidget);

        AssetViewerStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ArchiveViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *ArchiveViewerWidget)
    {
        ArchiveViewerWidget->setWindowTitle(QCoreApplication::translate("ArchiveViewerWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("ArchiveViewerWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ArchiveViewerWidget: public Ui_ArchiveViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCHIVEVIEWERWIDGET_H
