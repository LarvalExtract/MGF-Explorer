#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ArchiveViewer/ArchiveViewerWidget.h"

#include "Utilities/configfile.h"

#include <QMainWindow>
#include <QString>

#include <string>
#include <unordered_map>

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

    std::unordered_map<std::string, ArchiveViewer::ArchiveViewerWidget> ArchiveWidgets;
    ArchiveViewer::ArchiveViewerWidget* CurrentArchiveWidget = nullptr;

    void UpdateStatusBar() const;
    void AllTabsClosed();
};

#endif // MAINWINDOW_H
