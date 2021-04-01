#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "Utilities/ContextProvider/ServiceProvider.h"

#include <QFileDialog>
#include <QMessageBox>

#include <exception>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_AppConfig(std::filesystem::path("paths.ini"))
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
	ServiceProvider::Add<ResourceManager>(&m_ResourceManager);
	ServiceProvider::Add<ConfigFile>(&m_AppConfig);
	ServiceProvider::Add<Ogre::SceneManager>(m_OgreRoot->createSceneManager());
}

MainWindow::~MainWindow()
{
    m_Workspaces.clear();
    m_AppConfig.WriteOut();
    ServiceProvider::Destroy();

    delete ui;
}

void MainWindow::on_actionOpen_MGF_file_triggered()
{
    auto folder(m_AppConfig["Folders"]["DefaultMgfFolder"]);

    if (!std::filesystem::exists(folder))
        folder = "";

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Open MGF file", QString(folder.c_str()), tr("MechAssault MGF files (*.mgf)"));

    if (fileNames.size() > 0)
    {
		try
		{
            for (const auto& fileName : fileNames)
            {
                OpenMGFWorkspace(fileName);
            }

			std::filesystem::path parent(fileNames[0].toStdString());
			m_AppConfig["Folders"]["DefaultMgfFolder"] = parent.parent_path().u8string();
		}
		catch (const std::runtime_error& err)
		{
			QMessageBox::critical(this, "Error", err.what());
		}
    }
}

void MainWindow::on_actionClose_all_MGF_files_triggered()
{
    m_Workspaces.clear();

    AllTabsClosed();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    auto ws = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->widget(index));
    auto str = ws->MGFFile().GetFilePath();

    ui->tabWidget->removeTab(index);
    m_Workspaces.erase(str.toStdString());

    if (ui->tabWidget->count() == 0)
    {
        AllTabsClosed();
    }
}

void MainWindow::OpenMGFWorkspace(const QString &mgfFilePath)
{
    // file workspace is already open
    const auto& str = mgfFilePath.toStdString();

    if (m_Workspaces.find(str) != m_Workspaces.end())
    {
        int tabIndex = ui->tabWidget->indexOf(&m_Workspaces.at(str));
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
    else
    {
        auto ws_it = m_Workspaces.try_emplace(str, mgfFilePath, ui->tabWidget);
        auto ws = &(*ws_it.first).second;
        int newTabIndex = ui->tabWidget->addTab(ws,ws->MGFFile().GetFileName());

        ui->tabWidget->setCurrentIndex(newTabIndex);
    }

    if (ui->tabWidget->isHidden())
        ui->tabWidget->show();

    ui->actionTextures->setEnabled(true);
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == -1 || m_Workspaces.size() == 0)
    {
        m_pCurrentWorkspace = nullptr;
        return;
    }

    m_pCurrentWorkspace = static_cast<ArchiveViewer::ArchiveViewerWidget*>(ui->tabWidget->currentWidget());
    
    ui->actionFiles->setText(m_pCurrentWorkspace->MGFFile().GetFileName() + " file table...");

    UpdateStatusBar();
}

void MainWindow::InitialiseOgre()
{
    //m_OgreLogManager = std::make_unique<Ogre::LogManager>();
    //m_OgreLogManager->createLog("ogre.log", true, false, false);

    m_OgreRoot = std::make_unique<Ogre::Root>("plugins.cfg", "ogre.cfg", "");

    const auto& renderers = m_OgreRoot->getAvailableRenderers();

    if (renderers.size() == 0)
        throw std::runtime_error("No renderers found. Please ensure plugins.cfg is present in the application root.");

    m_OgreRoot->setRenderSystem(renderers[0]);
    m_OgreRoot->initialise(false);
}

void MainWindow::UpdateStatusBar()
{
    const auto& mgf = m_pCurrentWorkspace->MGFFile();

    const QLocale& loc = this->locale();

    QString labelText = mgf.GetArchiveVersion() == MGF::Version::MechAssault2LW ? "MechAssault 2: Lone Wolf | " : "MechAssault | ";
    labelText += mgf.GetFileName() + " | ";
    labelText += loc.formattedDataSize(mgf.GetFileSize()) + " | ";
    labelText += QString::number(mgf.GetFileCount()) + " files";

    ui->labelCurrentArchive->setText(labelText);
}

void MainWindow::AllTabsClosed()
{
    m_pCurrentWorkspace = nullptr;
    ui->tabWidget->hide();
    ui->labelCurrentArchive->clear();
    ui->labelCurrentItem->clear();
    ui->actionTextures->setEnabled(false);
    ui->actionFiles->setEnabled(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


