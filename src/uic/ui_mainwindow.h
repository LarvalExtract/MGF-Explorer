/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_MGF_file;
    QAction *actionClose_all_MGF_files;
    QAction *actionTextures;
    QAction *actionAbout;
    QAction *actionFiles;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *labelCurrentArchive;
    QLabel *labelCurrentItem;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuAbout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1600, 868);
        MainWindow->setTabShape(QTabWidget::Rounded);
        actionOpen_MGF_file = new QAction(MainWindow);
        actionOpen_MGF_file->setObjectName(QString::fromUtf8("actionOpen_MGF_file"));
        actionClose_all_MGF_files = new QAction(MainWindow);
        actionClose_all_MGF_files->setObjectName(QString::fromUtf8("actionClose_all_MGF_files"));
        actionTextures = new QAction(MainWindow);
        actionTextures->setObjectName(QString::fromUtf8("actionTextures"));
        actionTextures->setEnabled(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionFiles = new QAction(MainWindow);
        actionFiles->setObjectName(QString::fromUtf8("actionFiles"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMinimumSize(QSize(1600, 847));
        centralWidget->setMaximumSize(QSize(1600, 879));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setMinimumSize(QSize(127, 6));
        tabWidget->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(true);

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelCurrentArchive = new QLabel(centralWidget);
        labelCurrentArchive->setObjectName(QString::fromUtf8("labelCurrentArchive"));
        labelCurrentArchive->setMinimumSize(QSize(0, 13));
        labelCurrentArchive->setMaximumSize(QSize(620, 13));

        horizontalLayout->addWidget(labelCurrentArchive);

        labelCurrentItem = new QLabel(centralWidget);
        labelCurrentItem->setObjectName(QString::fromUtf8("labelCurrentItem"));

        horizontalLayout->addWidget(labelCurrentItem);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen_MGF_file);
        menuFile->addAction(actionClose_all_MGF_files);
        menuView->addAction(actionFiles);
        menuView->addAction(actionTextures);
        menuAbout->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_MGF_file->setText(QCoreApplication::translate("MainWindow", "Open MGF file...", nullptr));
        actionClose_all_MGF_files->setText(QCoreApplication::translate("MainWindow", "Close all MGF files", nullptr));
        actionTextures->setText(QCoreApplication::translate("MainWindow", "Textures...", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About Qt...", nullptr));
        actionFiles->setText(QCoreApplication::translate("MainWindow", "Files...", nullptr));
        labelCurrentArchive->setText(QString());
        labelCurrentItem->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
