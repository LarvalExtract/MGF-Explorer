/********************************************************************************
** Form generated from reading UI file 'PlainTextViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.4.1
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

class Ui_MGFPlainTextViewerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *MGFPlainTextViewerWidget)
    {
        if (MGFPlainTextViewerWidget->objectName().isEmpty())
            MGFPlainTextViewerWidget->setObjectName("MGFPlainTextViewerWidget");
        MGFPlainTextViewerWidget->resize(518, 518);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MGFPlainTextViewerWidget->sizePolicy().hasHeightForWidth());
        MGFPlainTextViewerWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(MGFPlainTextViewerWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(MGFPlainTextViewerWidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(plainTextEdit);


        retranslateUi(MGFPlainTextViewerWidget);

        QMetaObject::connectSlotsByName(MGFPlainTextViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *MGFPlainTextViewerWidget)
    {
        MGFPlainTextViewerWidget->setWindowTitle(QCoreApplication::translate("MGFPlainTextViewerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MGFPlainTextViewerWidget: public Ui_MGFPlainTextViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAINTEXTVIEWERWIDGET_H
