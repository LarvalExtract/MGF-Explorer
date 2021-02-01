/********************************************************************************
** Form generated from reading UI file 'PlainTextViewerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
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
            PlainTextViewerWidget->setObjectName(QString::fromUtf8("PlainTextViewerWidget"));
        PlainTextViewerWidget->resize(518, 518);
        horizontalLayout = new QHBoxLayout(PlainTextViewerWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(PlainTextViewerWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
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
