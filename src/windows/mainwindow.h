#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/mgfworkspace.h"
#include "mgfresourcemanager.h"
#include "ui_windowfiletable.h"

#include "utils/Contexts.h"
#include "utils/configfile.h"
#include "utils/events/IEventListener.h"

#include <QMainWindow>
#include <QString>

#include <string>
#include <unordered_map>
#include <memory>

#include <Ogre.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IEventListener
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OpenMGFWorkspace(const QString& mgfFilePath);

    void OnNotify(EventData* data) override;

private slots:
    void on_actionOpen_MGF_file_triggered();
    void on_actionClose_all_MGF_files_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);   
    void on_actionFiles_triggered();
    void on_actionTextures_triggered();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    std::unordered_map<std::string, MGFWorkspace> m_Workspaces;
    MGFWorkspace* m_pCurrentWorkspace = nullptr;

    std::unique_ptr<Ogre::LogManager> m_OgreLogManager;
    std::unique_ptr<Ogre::Root> m_OgreRoot;

    MGFResourceManager m_ResourceManager;
    ConfigFile m_AppConfig;
   
private:
    void InitialiseOgre();   
    void LaunchFileExtractDialog();
    void UpdateStatusBar(const MGF::File* selectedItem);
    void AllTabsClosed();
};

#endif // MAINWINDOW_H
