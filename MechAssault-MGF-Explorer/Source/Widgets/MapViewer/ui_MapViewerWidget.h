/********************************************************************************
** Form generated from reading UI file 'MapViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPVIEWERWIDGET_H
#define UI_MAPVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFMapViewerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *MapViewerWidgetContainerLayout;
    QHBoxLayout *terrainPositionInputLayout;
    QLabel *terrainPositionLabel;
    QDoubleSpinBox *terrainPositionXInputSpinbox;
    QDoubleSpinBox *terrainPositionYInputSpinbox;
    QDoubleSpinBox *terrainPositionZInputSpinbox;
    QSpacerItem *terrainPositionSpacer;
    QHBoxLayout *terrainRotationInputLayout;
    QLabel *terrainRotationLabel;
    QDoubleSpinBox *terrainRotationInputSpinbox;
    QSpacerItem *terrainRotationSpacer;
    QHBoxLayout *terrainScaleInputLayout;
    QLabel *terrainScaleLabel;
    QDoubleSpinBox *terrainScaleInputSpinbox;
    QSpacerItem *terrainScaleSpacer;
    QVBoxLayout *verticalLayout_2;
    QTreeView *entityTreeView;
    QTableView *attributeTableView;

    void setupUi(QWidget *MGFMapViewerWidget)
    {
        if (MGFMapViewerWidget->objectName().isEmpty())
            MGFMapViewerWidget->setObjectName("MGFMapViewerWidget");
        MGFMapViewerWidget->resize(1089, 825);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MGFMapViewerWidget->sizePolicy().hasHeightForWidth());
        MGFMapViewerWidget->setSizePolicy(sizePolicy);
        MGFMapViewerWidget->setMinimumSize(QSize(50, 0));
        horizontalLayout = new QHBoxLayout(MGFMapViewerWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        MapViewerWidgetContainerLayout = new QVBoxLayout();
        MapViewerWidgetContainerLayout->setObjectName("MapViewerWidgetContainerLayout");
        MapViewerWidgetContainerLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        terrainPositionInputLayout = new QHBoxLayout();
        terrainPositionInputLayout->setObjectName("terrainPositionInputLayout");
        terrainPositionLabel = new QLabel(MGFMapViewerWidget);
        terrainPositionLabel->setObjectName("terrainPositionLabel");
        terrainPositionLabel->setMinimumSize(QSize(50, 0));
        terrainPositionLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        terrainPositionInputLayout->addWidget(terrainPositionLabel);

        terrainPositionXInputSpinbox = new QDoubleSpinBox(MGFMapViewerWidget);
        terrainPositionXInputSpinbox->setObjectName("terrainPositionXInputSpinbox");
        terrainPositionXInputSpinbox->setMinimum(-999999.000000000000000);
        terrainPositionXInputSpinbox->setMaximum(999999.000000000000000);

        terrainPositionInputLayout->addWidget(terrainPositionXInputSpinbox);

        terrainPositionYInputSpinbox = new QDoubleSpinBox(MGFMapViewerWidget);
        terrainPositionYInputSpinbox->setObjectName("terrainPositionYInputSpinbox");
        terrainPositionYInputSpinbox->setMinimum(-999999.000000000000000);
        terrainPositionYInputSpinbox->setMaximum(999999.000000000000000);

        terrainPositionInputLayout->addWidget(terrainPositionYInputSpinbox);

        terrainPositionZInputSpinbox = new QDoubleSpinBox(MGFMapViewerWidget);
        terrainPositionZInputSpinbox->setObjectName("terrainPositionZInputSpinbox");
        terrainPositionZInputSpinbox->setMinimum(-999999.000000000000000);
        terrainPositionZInputSpinbox->setMaximum(999999.000000000000000);

        terrainPositionInputLayout->addWidget(terrainPositionZInputSpinbox);

        terrainPositionSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        terrainPositionInputLayout->addItem(terrainPositionSpacer);


        MapViewerWidgetContainerLayout->addLayout(terrainPositionInputLayout);

        terrainRotationInputLayout = new QHBoxLayout();
        terrainRotationInputLayout->setObjectName("terrainRotationInputLayout");
        terrainRotationInputLayout->setContentsMargins(0, -1, -1, -1);
        terrainRotationLabel = new QLabel(MGFMapViewerWidget);
        terrainRotationLabel->setObjectName("terrainRotationLabel");
        terrainRotationLabel->setMinimumSize(QSize(50, 0));
        terrainRotationLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        terrainRotationInputLayout->addWidget(terrainRotationLabel);

        terrainRotationInputSpinbox = new QDoubleSpinBox(MGFMapViewerWidget);
        terrainRotationInputSpinbox->setObjectName("terrainRotationInputSpinbox");
        terrainRotationInputSpinbox->setMaximum(360.000000000000000);

        terrainRotationInputLayout->addWidget(terrainRotationInputSpinbox);

        terrainRotationSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        terrainRotationInputLayout->addItem(terrainRotationSpacer);


        MapViewerWidgetContainerLayout->addLayout(terrainRotationInputLayout);

        terrainScaleInputLayout = new QHBoxLayout();
        terrainScaleInputLayout->setObjectName("terrainScaleInputLayout");
        terrainScaleLabel = new QLabel(MGFMapViewerWidget);
        terrainScaleLabel->setObjectName("terrainScaleLabel");
        terrainScaleLabel->setMinimumSize(QSize(50, 0));
        terrainScaleLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        terrainScaleInputLayout->addWidget(terrainScaleLabel);

        terrainScaleInputSpinbox = new QDoubleSpinBox(MGFMapViewerWidget);
        terrainScaleInputSpinbox->setObjectName("terrainScaleInputSpinbox");
        terrainScaleInputSpinbox->setMaximum(100.000000000000000);

        terrainScaleInputLayout->addWidget(terrainScaleInputSpinbox);

        terrainScaleSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        terrainScaleInputLayout->addItem(terrainScaleSpacer);


        MapViewerWidgetContainerLayout->addLayout(terrainScaleInputLayout);


        horizontalLayout->addLayout(MapViewerWidgetContainerLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        entityTreeView = new QTreeView(MGFMapViewerWidget);
        entityTreeView->setObjectName("entityTreeView");
        entityTreeView->setMinimumSize(QSize(400, 0));
        entityTreeView->setMaximumSize(QSize(400, 16777215));

        verticalLayout_2->addWidget(entityTreeView);

        attributeTableView = new QTableView(MGFMapViewerWidget);
        attributeTableView->setObjectName("attributeTableView");
        attributeTableView->setMinimumSize(QSize(400, 0));
        attributeTableView->setMaximumSize(QSize(400, 16777215));

        verticalLayout_2->addWidget(attributeTableView);


        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(MGFMapViewerWidget);

        QMetaObject::connectSlotsByName(MGFMapViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFMapViewerWidget)
    {
        MGFMapViewerWidget->setWindowTitle(QCoreApplication::translate("MGFMapViewerWidget", "Form", nullptr));
        terrainPositionLabel->setText(QCoreApplication::translate("MGFMapViewerWidget", "Position", nullptr));
        terrainRotationLabel->setText(QCoreApplication::translate("MGFMapViewerWidget", "Rotation", nullptr));
        terrainScaleLabel->setText(QCoreApplication::translate("MGFMapViewerWidget", "Scale", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFMapViewerWidget: public Ui_MGFMapViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPVIEWERWIDGET_H
