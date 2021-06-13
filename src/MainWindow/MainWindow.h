#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ArchiveViewer/ArchiveViewerWidget.h"

#include "Utilities/ContextProvider/ServiceProvider.h"
#include "Utilities/configfile.h"

#include "MGF/AssetManager.h"

#include <QMainWindow>
#include <QString>

#include <string>
#include <unordered_map>
#include <memory>

#include <Ogre.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OpenMGFWorkspace(const QString& mgfFilePath);

private slots:
    void on_actionOpen_MGF_file_triggered();
    void on_actionClose_all_MGF_files_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    std::unordered_map<std::string, ArchiveViewer::ArchiveViewerWidget> m_Workspaces;
    ArchiveViewer::ArchiveViewerWidget* m_pCurrentWorkspace = nullptr;

    std::unique_ptr<Ogre::LogManager> m_OgreLogManager;
    std::unique_ptr<Ogre::Root> m_OgreRoot;

    MGF::AssetManager AssetManager;
   
private:
    void InitialiseOgre();
    void UpdateStatusBar();
    void AllTabsClosed();
};

#endif // MAINWINDOW_H
