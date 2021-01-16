#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mgfresourcemanager.h"
#include "utils/events/EventSelectedItemChanged.h"

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

    Contexts::Initialise();
    Contexts::Set<MGFResourceManager>(&m_ResourceManager);
    Contexts::Set<ConfigFile>(&m_AppConfig);
}

MainWindow::~MainWindow()
{
    m_Workspaces.clear();
    m_AppConfig.WriteOut();
    Contexts::Destroy();

    delete ui;
}

void MainWindow::on_actionOpen_MGF_file_triggered()
{
    auto folder(m_AppConfig["Folders"]["DefaultMgfFolder"]);

    if (!std::filesystem::exists(folder))
        folder = "";

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Open MGF file", QString(folder.c_str()), tr("MechAssault MGF files (*.mgf)"));

    try
    {
        for (const auto& fileName : fileNames)
            OpenMGFWorkspace(fileName);
    }
    catch (const std::runtime_error& err)
    {
        QMessageBox::critical(this, "Error", err.what());
    }

    std::filesystem::path parent(fileNames[0].toStdString());

    m_AppConfig["Folders"]["DefaultMgfFolder"] = parent.parent_path().u8string();
}

void MainWindow::on_actionClose_all_MGF_files_triggered()
{
    m_Workspaces.clear();

    AllTabsClosed();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    auto ws = static_cast<MGFWorkspace*>(ui->tabWidget->widget(index));
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

        ws->AddListener(this);

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

    m_pCurrentWorkspace = static_cast<MGFWorkspace*>(ui->tabWidget->currentWidget());
    UpdateStatusBar(m_pCurrentWorkspace->SelectedItem());
    
    ui->actionFiles->setText(m_pCurrentWorkspace->MGFFile().GetFileName() + " file table...");
}

void MainWindow::OnNotify(EventData* data)
{
    auto itemEvent = static_cast<EventSelectedItemChanged*>(data);
    UpdateStatusBar(itemEvent->MgfItem);
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

void MainWindow::UpdateStatusBar(const MGF::File* selectedItem)
{
    const auto& mgf = m_pCurrentWorkspace->MGFFile();

    const QLocale& loc = this->locale();

    QString labelText = mgf.GetArchiveVersion() == 4 ? "MechAssault 2: Lone Wolf | " : "MechAssault | ";
    labelText += mgf.GetFileName() + " | ";
    labelText += loc.formattedDataSize(mgf.GetFileSize()) + " | ";
    labelText += QString::number(mgf.GetFileCount()) + " files";

    ui->labelCurrentArchive->setText(labelText);

    if (selectedItem)
    {
        if (selectedItem->IsFile())
        {
            QString text = selectedItem->FilePath().u8string().c_str();
            text += " | ID: " + QString::number(selectedItem->GUID(), 16);
            text += " | Index: " + QString::number(selectedItem->Index());
            text += " | Offset: " + QString::number(selectedItem->FileOffset());
            text += " | Length: " + QString::number(selectedItem->FileLength()) + " bytes";
            ui->labelCurrentItem->setText(text);
        }
        else
        {
            ui->labelCurrentItem->clear();
        }
    }
    else
    {
        ui->labelCurrentItem->clear();
    }
}

void MainWindow::AllTabsClosed()
{
    m_pCurrentWorkspace = nullptr;
    ui->tabWidget->hide();
    ui->labelCurrentArchive->clear();
    ui->labelCurrentItem->clear();
    ui->actionTextures->setEnabled(false);
    ui->actionFiles->setEnabled(false);

    m_ResourceManager.Clear();
}

void MainWindow::on_actionFiles_triggered()
{
    m_pCurrentWorkspace->ShowFileTableWindow();
}

void MainWindow::on_actionTextures_triggered()
{
    
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


