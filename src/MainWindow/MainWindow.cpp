#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Utilities/ContextProvider/ServiceProvider.h"

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

    try
    {
        InitialiseOgre();
    }
    catch (const std::exception& error)
    {
        QString msg = error.what();
        msg += "\n\nYou will not be able to preview models and textures.";
        msg += "\nYou will still be able to open, explore, and extract the contents of MGF files.";

        QMessageBox::critical(this, "Failed to initialise renderer",
                              msg);
    }

    ServiceProvider::Initialise();
	ServiceProvider::Add<MGF::AssetManager>(&AssetManager);
	ServiceProvider::Add<Ogre::SceneManager>(m_OgreRoot->createSceneManager());
}

MainWindow::~MainWindow()
{
    ArchiveWidgets.clear();
    ServiceProvider::Destroy();
    delete ui;
}

void MainWindow::on_actionOpen_MGF_file_triggered()
{
    if (const auto fileNames = QFileDialog::getOpenFileNames(this, "Open MGF file", QString{}, tr("MechAssault MGF files (*.mgf)")); !fileNames.empty())
    {
		try
		{
            for (const auto& fileName : fileNames)
            {
                OpenMGFWorkspace(fileName);
            }
		}
		catch (const std::runtime_error& err)
		{
			QMessageBox::critical(this, "Error", err.what());
		}
    }
}

void MainWindow::on_actionClose_all_MGF_files_triggered()
{
    ArchiveWidgets.clear();

    AllTabsClosed();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    auto ws = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->widget(index));
    auto str = QString(ws->MGFArchive().Path.u8string().c_str());

    ui->tabWidget->removeTab(index);
    ArchiveWidgets.erase(str.toStdString());

    if (ui->tabWidget->count() == 0)
    {
        AllTabsClosed();
    }
}

void MainWindow::OpenMGFWorkspace(const QString &mgfFilePath)
{
    std::filesystem::path path = mgfFilePath.toStdString();

    if (const auto str = mgfFilePath.toStdString(); ArchiveWidgets.find(str) != ArchiveWidgets.end())
    {
        int tabIndex = ui->tabWidget->indexOf(&ArchiveWidgets.at(str));
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
    else
    {
        ArchiveWidgets.try_emplace(str, path, ui->tabWidget);

        auto& widget = ArchiveWidgets.at(str);
        const QString tabTitle = path.filename().u8string().c_str();

        int newTabIndex = ui->tabWidget->addTab(&widget, tabTitle);
        ui->tabWidget->setCurrentIndex(newTabIndex);
    }
    
    ui->tabWidget->show();
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == -1 || ArchiveWidgets.empty())
    {
        CurrentArchiveWidget = nullptr;
        return;
    }

    CurrentArchiveWidget = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->currentWidget());

    UpdateStatusBar();
}

void MainWindow::InitialiseOgre()
{
    m_OgreRoot = std::make_unique<Ogre::Root>("plugins.cfg", "ogre.cfg", "");

    const auto& renderers = m_OgreRoot->getAvailableRenderers();

    if (renderers.empty())
        throw std::runtime_error("No renderers found. Please ensure plugins.cfg is present in the application root.");

    m_OgreRoot->setRenderSystem(renderers[0]);
    m_OgreRoot->initialise(false);
}

void MainWindow::UpdateStatusBar()
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


