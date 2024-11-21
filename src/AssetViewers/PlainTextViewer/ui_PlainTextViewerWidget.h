/********************************************************************************
** Form generated from reading UI file 'PlainTextViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAINTEXTVIEWERWIDGET_H
#define UI_PLAINTEXTVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlainTextViewerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *PlainTextViewerWidget)
    {
        if (PlainTextViewerWidget->objectName().isEmpty())
            PlainTextViewerWidget->setObjectName("PlainTextViewerWidget");
        PlainTextViewerWidget->resize(518, 518);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PlainTextViewerWidget->sizePolicy().hasHeightForWidth());
        PlainTextViewerWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(PlainTextViewerWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(PlainTextViewerWidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(plainTextEdit);


        retranslateUi(PlainTextViewerWidget);

        QMetaObject::connectSlotsByName(PlainTextViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *PlainTextViewerWidget)
    {
        PlainTextViewerWidget->setWindowTitle(QCoreApplication::translate("PlainTextViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlainTextViewerWidget: public Ui_PlainTextViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAINTEXTVIEWERWIDGET_H
