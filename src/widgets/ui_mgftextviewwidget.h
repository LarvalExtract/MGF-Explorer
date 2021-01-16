/********************************************************************************
** Form generated from reading UI file 'mgftextviewwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MGFTEXTVIEWWIDGET_H
#define UI_MGFTEXTVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MGFTextViewWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *MGFTextViewWidget)
    {
        if (MGFTextViewWidget->objectName().isEmpty())
            MGFTextViewWidget->setObjectName(QString::fromUtf8("MGFTextViewWidget"));
        MGFTextViewWidget->resize(518, 518);
        horizontalLayout = new QHBoxLayout(MGFTextViewWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(MGFTextViewWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(plainTextEdit);


        retranslateUi(MGFTextViewWidget);

        QMetaObject::connectSlotsByName(MGFTextViewWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFTextViewWidget)
    {
        MGFTextViewWidget->setWindowTitle(QCoreApplication::translate("MGFTextViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFTextViewWidget: public Ui_MGFTextViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MGFTEXTVIEWWIDGET_H
