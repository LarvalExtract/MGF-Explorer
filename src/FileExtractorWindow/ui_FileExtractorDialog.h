/********************************************************************************
** Form generated from reading UI file 'FileExtractorDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.0.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEEXTRACTORDIALOG_H
#define UI_FILEEXTRACTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileExtractorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelOutput;
    QLineEdit *destinationFolderPath;
    QPushButton *browseButton;
    QLabel *labelTask;
    QHBoxLayout *horizontalLayout_2;
    QTableView *tableFileQueue;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBoxFilepaths;
    QCheckBox *checkBoxOverwrite;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelProgress;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FileExtractorDialog)
    {
        if (FileExtractorDialog->objectName().isEmpty())
            FileExtractorDialog->setObjectName(QString::fromUtf8("FileExtractorDialog"));
        FileExtractorDialog->setWindowModality(Qt::ApplicationModal);
        FileExtractorDialog->resize(810, 585);
        FileExtractorDialog->setModal(true);
        verticalLayout = new QVBoxLayout(FileExtractorDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelOutput = new QLabel(FileExtractorDialog);
        labelOutput->setObjectName(QString::fromUtf8("labelOutput"));

        horizontalLayout->addWidget(labelOutput);

        destinationFolderPath = new QLineEdit(FileExtractorDialog);
        destinationFolderPath->setObjectName(QString::fromUtf8("destinationFolderPath"));
        destinationFolderPath->setReadOnly(true);

        horizontalLayout->addWidget(destinationFolderPath);

        browseButton = new QPushButton(FileExtractorDialog);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));

        horizontalLayout->addWidget(browseButton);


        verticalLayout->addLayout(horizontalLayout);

        labelTask = new QLabel(FileExtractorDialog);
        labelTask->setObjectName(QString::fromUtf8("labelTask"));
        labelTask->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(labelTask);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tableFileQueue = new QTableView(FileExtractorDialog);
        tableFileQueue->setObjectName(QString::fromUtf8("tableFileQueue"));
        tableFileQueue->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableFileQueue->setGridStyle(Qt::NoPen);
        tableFileQueue->horizontalHeader()->setVisible(true);
        tableFileQueue->verticalHeader()->setVisible(false);
        tableFileQueue->verticalHeader()->setMinimumSectionSize(22);
        tableFileQueue->verticalHeader()->setDefaultSectionSize(22);

        horizontalLayout_2->addWidget(tableFileQueue);

        groupBox = new QGroupBox(FileExtractorDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(170, 0));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        checkBoxFilepaths = new QCheckBox(groupBox);
        checkBoxFilepaths->setObjectName(QString::fromUtf8("checkBoxFilepaths"));
        checkBoxFilepaths->setChecked(true);

        verticalLayout_2->addWidget(checkBoxFilepaths);

        checkBoxOverwrite = new QCheckBox(groupBox);
        checkBoxOverwrite->setObjectName(QString::fromUtf8("checkBoxOverwrite"));

        verticalLayout_2->addWidget(checkBoxOverwrite);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout_2);

        progressBar = new QProgressBar(FileExtractorDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        verticalLayout->addWidget(progressBar);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelProgress = new QLabel(FileExtractorDialog);
        labelProgress->setObjectName(QString::fromUtf8("labelProgress"));

        horizontalLayout_3->addWidget(labelProgress);

        buttonBox = new QDialogButtonBox(FileExtractorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Discard|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        horizontalLayout_3->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(FileExtractorDialog);

        QMetaObject::connectSlotsByName(FileExtractorDialog);
    } // setupUi

    void retranslateUi(QDialog *FileExtractorDialog)
    {
        FileExtractorDialog->setWindowTitle(QCoreApplication::translate("FileExtractorDialog", "Dialog", nullptr));
        labelOutput->setText(QCoreApplication::translate("FileExtractorDialog", "Output", nullptr));
        browseButton->setText(QCoreApplication::translate("FileExtractorDialog", "Browse", nullptr));
        labelTask->setText(QString());
        groupBox->setTitle(QCoreApplication::translate("FileExtractorDialog", "Options", nullptr));
        checkBoxFilepaths->setText(QCoreApplication::translate("FileExtractorDialog", "Keep file paths", nullptr));
        checkBoxOverwrite->setText(QCoreApplication::translate("FileExtractorDialog", "Overwrite existing files", nullptr));
        labelProgress->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FileExtractorDialog: public Ui_FileExtractorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEEXTRACTORDIALOG_H
