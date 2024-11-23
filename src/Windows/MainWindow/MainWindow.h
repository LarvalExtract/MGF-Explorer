#pragma once

#include <QMainWindow>
#include <QString>

class MGFArchive;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void OpenMGFWorkspace(std::shared_ptr<MGFArchive> mgfArchive, const std::filesystem::path* assetPath = nullptr);

private slots:
    void on_actionOpen_MGF_file_triggered();
    void on_actionClose_all_MGF_files_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    void AllTabsClosed();
};

