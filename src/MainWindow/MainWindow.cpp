#include "MainWindow.h"
#include "ui_MainWindow.h"

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
    ArchiveWidgets.clear();
    delete ui;
}

void MainWindow::on_actionOpen_MGF_file_triggered()
{
    QSettings settings;
    const QString defaultMgfFolderKey = "SavedMgfFolder";

    if (const auto fileNames = QFileDialog::getOpenFileNames(this, "Open MGF file", settings.value(defaultMgfFolderKey).toString(), tr("MechAssault MGF files (*.mgf)")); !fileNames.empty())
    {
        std::vector<std::exception> errors;

    	for (const auto& fileName : fileNames)
    	{
    		try
    		{
                const std::filesystem::path path(fileName.toLatin1().constData());
    			OpenMGFWorkspace(path);
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
        const QString folder(mgfFolder.parent_path().string().c_str());
        
        settings.setValue(defaultMgfFolderKey, folder);
    }
}

void MainWindow::on_actionClose_all_MGF_files_triggered()
{
    ArchiveWidgets.clear();

    AllTabsClosed();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    const auto ws = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->widget(index));
    const auto str = ws->MGFArchive().Path.string();

    ui->tabWidget->removeTab(index);
    ArchiveWidgets.erase(str);

    if (ui->tabWidget->count() == 0)
    {
        AllTabsClosed();
    }
}

void MainWindow::OpenMGFWorkspace(const std::filesystem::path& mgfFilePath, const std::filesystem::path* assetPath)
{
    const auto str = mgfFilePath.string();
    if (ArchiveWidgets.contains(str))
    {
        const int tabIndex = ui->tabWidget->indexOf(&ArchiveWidgets.at(str));
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
    else
    {
        ArchiveWidgets.try_emplace(str, mgfFilePath, ui->tabWidget);

        const int newTabIndex = ui->tabWidget->addTab(&ArchiveWidgets.at(str), QString{mgfFilePath.filename().string().c_str()});
        ui->tabWidget->setCurrentIndex(newTabIndex);
    }

    if (assetPath)
    {
        ArchiveWidgets.at(str).OpenAsset(*assetPath);
    }
    
    ui->tabWidget->show();
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index >= 0 && index < ArchiveWidgets.size())
    {
		CurrentArchiveWidget = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->currentWidget());
		UpdateStatusBar();
    }
    else
    {
        CurrentArchiveWidget = nullptr;
    }
}

void MainWindow::UpdateStatusBar() const
{
    const auto& mgf = CurrentArchiveWidget->MGFArchive();
    const auto& loc = this->locale();

    const auto labelText = QString("%1 | %2 | %3 | %4 files").arg(
        MGF::ToString(mgf.GetVersion()),
        mgf.Path.filename().u8string().c_str(),
        loc.formattedDataSize(mgf.GetFileSize()),
        QString::number(mgf.GetFileCount())
    );

    ui->labelCurrentArchive->setText(labelText);
}

void MainWindow::AllTabsClosed()
{
    CurrentArchiveWidget = nullptr;
    ui->tabWidget->hide();
    ui->labelCurrentArchive->clear();
    ui->actionTextures->setEnabled(false);
    ui->actionFiles->setEnabled(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


