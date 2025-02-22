/********************************************************************************
** Form generated from reading UI file 'ModelViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWERWIDGET_H
#define UI_MODELVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFModelViewerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *frameLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QCheckBox *enableSceneLightsCheckBox;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QSlider *lightPositionSliderXInput;
    QSlider *lightPositionSliderYInput;
    QSlider *lightPositionSliderZInput;
    QSpacerItem *verticalSpacer;
    QTabWidget *tabWidget;
    QWidget *tabNodes;
    QHBoxLayout *horizontalLayout_5;
    QTreeView *nodeTreeView;
    QWidget *tabAnimations;
    QHBoxLayout *horizontalLayout_4;
    QTableView *animTableView;
    QWidget *tabMeshes;
    QHBoxLayout *horizontalLayout_2;
    QTableView *meshTableView;
    QWidget *tabMaterials;
    QHBoxLayout *horizontalLayout;
    QTableView *materialTableView;

    void setupUi(QWidget *MGFModelViewerWidget)
    {
        if (MGFModelViewerWidget->objectName().isEmpty())
            MGFModelViewerWidget->setObjectName("MGFModelViewerWidget");
        MGFModelViewerWidget->resize(1091, 670);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MGFModelViewerWidget->sizePolicy().hasHeightForWidth());
        MGFModelViewerWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MGFModelViewerWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(MGFModelViewerWidget);
        frame->setObjectName("frame");
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frameLayout = new QVBoxLayout(frame);
        frameLayout->setObjectName("frameLayout");
        frameLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        enableSceneLightsCheckBox = new QCheckBox(frame);
        enableSceneLightsCheckBox->setObjectName("enableSceneLightsCheckBox");
        enableSceneLightsCheckBox->setChecked(true);

        horizontalLayout_3->addWidget(enableSceneLightsCheckBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        verticalSpacer_2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer_2);

        lightPositionSliderXInput = new QSlider(frame);
        lightPositionSliderXInput->setObjectName("lightPositionSliderXInput");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lightPositionSliderXInput->sizePolicy().hasHeightForWidth());
        lightPositionSliderXInput->setSizePolicy(sizePolicy1);
        lightPositionSliderXInput->setMinimumSize(QSize(200, 0));
        lightPositionSliderXInput->setMinimum(-100);
        lightPositionSliderXInput->setMaximum(100);
        lightPositionSliderXInput->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(lightPositionSliderXInput);

        lightPositionSliderYInput = new QSlider(frame);
        lightPositionSliderYInput->setObjectName("lightPositionSliderYInput");
        sizePolicy1.setHeightForWidth(lightPositionSliderYInput->sizePolicy().hasHeightForWidth());
        lightPositionSliderYInput->setSizePolicy(sizePolicy1);
        lightPositionSliderYInput->setMinimumSize(QSize(200, 0));
        lightPositionSliderYInput->setMinimum(-100);
        lightPositionSliderYInput->setMaximum(100);
        lightPositionSliderYInput->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(lightPositionSliderYInput);

        lightPositionSliderZInput = new QSlider(frame);
        lightPositionSliderZInput->setObjectName("lightPositionSliderZInput");
        sizePolicy1.setHeightForWidth(lightPositionSliderZInput->sizePolicy().hasHeightForWidth());
        lightPositionSliderZInput->setSizePolicy(sizePolicy1);
        lightPositionSliderZInput->setMinimumSize(QSize(200, 0));
        lightPositionSliderZInput->setMinimum(-100);
        lightPositionSliderZInput->setMaximum(100);
        lightPositionSliderZInput->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(lightPositionSliderZInput);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout_2);


        frameLayout->addLayout(horizontalLayout_3);

        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMaximumSize(QSize(16777215, 250));
        tabNodes = new QWidget();
        tabNodes->setObjectName("tabNodes");
        horizontalLayout_5 = new QHBoxLayout(tabNodes);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(2, 2, 2, 2);
        nodeTreeView = new QTreeView(tabNodes);
        nodeTreeView->setObjectName("nodeTreeView");
        nodeTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed);

        horizontalLayout_5->addWidget(nodeTreeView);

        tabWidget->addTab(tabNodes, QString());
        tabAnimations = new QWidget();
        tabAnimations->setObjectName("tabAnimations");
        tabAnimations->setMinimumSize(QSize(1083, 0));
        horizontalLayout_4 = new QHBoxLayout(tabAnimations);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        animTableView = new QTableView(tabAnimations);
        animTableView->setObjectName("animTableView");
        animTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        animTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        animTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        animTableView->setShowGrid(false);
        animTableView->verticalHeader()->setVisible(false);
        animTableView->verticalHeader()->setMinimumSectionSize(22);
        animTableView->verticalHeader()->setDefaultSectionSize(22);

        horizontalLayout_4->addWidget(animTableView);

        tabWidget->addTab(tabAnimations, QString());
        tabMeshes = new QWidget();
        tabMeshes->setObjectName("tabMeshes");
        horizontalLayout_2 = new QHBoxLayout(tabMeshes);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        meshTableView = new QTableView(tabMeshes);
        meshTableView->setObjectName("meshTableView");
        meshTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        meshTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        meshTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        meshTableView->setShowGrid(false);
        meshTableView->verticalHeader()->setVisible(false);
        meshTableView->verticalHeader()->setMinimumSectionSize(22);
        meshTableView->verticalHeader()->setDefaultSectionSize(22);

        horizontalLayout_2->addWidget(meshTableView);

        tabWidget->addTab(tabMeshes, QString());
        tabMaterials = new QWidget();
        tabMaterials->setObjectName("tabMaterials");
        horizontalLayout = new QHBoxLayout(tabMaterials);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        materialTableView = new QTableView(tabMaterials);
        materialTableView->setObjectName("materialTableView");
        materialTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        materialTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        materialTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        materialTableView->setShowGrid(false);
        materialTableView->verticalHeader()->setVisible(false);
        materialTableView->verticalHeader()->setMinimumSectionSize(22);
        materialTableView->verticalHeader()->setDefaultSectionSize(22);

        horizontalLayout->addWidget(materialTableView);

        tabWidget->addTab(tabMaterials, QString());

        frameLayout->addWidget(tabWidget);


        verticalLayout->addWidget(frame);


        retranslateUi(MGFModelViewerWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MGFModelViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFModelViewerWidget)
    {
        MGFModelViewerWidget->setWindowTitle(QCoreApplication::translate("MGFModelViewerWidget", "Form", nullptr));
        enableSceneLightsCheckBox->setText(QCoreApplication::translate("MGFModelViewerWidget", "Enable lights", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabNodes), QCoreApplication::translate("MGFModelViewerWidget", "Nodes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAnimations), QCoreApplication::translate("MGFModelViewerWidget", "Animations", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMeshes), QCoreApplication::translate("MGFModelViewerWidget", "Meshes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMaterials), QCoreApplication::translate("MGFModelViewerWidget", "Materials", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFModelViewerWidget: public Ui_MGFModelViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWERWIDGET_H
