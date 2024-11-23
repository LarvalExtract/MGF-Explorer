#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MGFExplorerApplication.h"
#include "MGF/MGFArchive.h"
#include "Widgets/ArchiveViewer/ArchiveViewerWidget.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

#include <exception>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MechAssault MGF Explorer");
    this->showMaximized();
    ui->tabWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_MGF_file_triggered()
{
    if (const auto fileNames = QFileDialog::getOpenFileNames(this, "Open MGF file", qApp->GetMgfFolderFromAppSettings().u8string().c_str(), tr("MechAssault MGF files (*.mgf)")); !fileNames.empty())
    {
        std::vector<std::exception> errors;

    	for (const auto& fileName : fileNames)
    	{
    		try
    		{
                const std::filesystem::path path(fileName.toLatin1().constData());

                if (std::shared_ptr<MGFArchive> mgfArchive = qApp->GetMgfArchive(path))
                {
                    OpenMGFWorkspace(mgfArchive);
                }
    		}
            catch (const std::runtime_error& err)
            {
                errors.push_back(err);
            }
    	}
		
		if (!errors.empty())
		{
            QString message;
            for (const auto& err : errors)
            {
                message.append(err.what()) + '\n';
            }
            QMessageBox::critical(this, QString("Could not open %1 MGF file%2").arg(QString::number(errors.size()), errors.size() > 1 ? "s" : ""), message);
		}

        const std::filesystem::path mgfFolder(fileNames.first().toStdString());
        
        qApp->SetMgfFolderAppSetting(mgfFolder.parent_path());
    }
}

void MainWindow::on_actionClose_all_MGF_files_triggered()
{
    AllTabsClosed();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    delete ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);

    if (ui->tabWidget->count() == 0)
    {
        AllTabsClosed();
    }
}

void MainWindow::OpenMGFWorkspace(std::shared_ptr<MGFArchive> mgfArchive, const std::filesystem::path* assetPath)
{
    const std::string mgfArchiveName = mgfArchive->Path.filename().string();

    int index = -1;
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        if (MGFArchiveViewerWidget* archiveViewer = static_cast<MGFArchiveViewerWidget*>(ui->tabWidget->widget(i)))
        {
            if (archiveViewer->GetMgfArchive().Path == mgfArchive->Path)
            {
                index = i;
                break;
            }
        }
    }

    if (index == -1)
    {
        MGFArchiveViewerWidget* archiveViewerWidget = new MGFArchiveViewerWidget(mgfArchive, this);
        index = ui->tabWidget->addTab(archiveViewerWidget, QString{ mgfArchiveName.c_str() });
    }

    ui->tabWidget->setCurrentIndex(index);
    ui->tabWidget->show();
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index >= 0 && index < ui->tabWidget->count())
    {
        const auto& mgf = static_cast<MGFArchiveViewerWidget*>(ui->tabWidget->currentWidget())->GetMgfArchive();
        const auto& loc = this->locale();

        const auto labelText = QString("%1 | %2 | %3 | %4 files").arg(
            ToString(mgf.GetVersion()),
            mgf.Path.filename().u8string().c_str(),
            loc.formattedDataSize(mgf.GetFileSize()),
            QString::number(mgf.GetFileCount())
        );

        ui->labelCurrentArchive->setText(labelText);
    }
}

void MainWindow::AllTabsClosed()
{
    ui->tabWidget->hide();
    ui->labelCurrentArchive->clear();
    ui->actionTextures->setEnabled(false);
    ui->actionFiles->setEnabled(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


