#include "fileextractdialog.h"
#include "ui_fileextractdialog.h"
#include "mgf/mgfarchive.h"

#include "utils/configfile.h"
#include "utils/Contexts.h"

#include <QFileDialog>
#include <QMessageBox>

#include <filesystem>
#include <thread>

FileExtractDialog::FileExtractDialog(const QModelIndexList& selection, const MGFArchive& archive, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileExtractDialog),
    FileListModel(m_Extractor)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    m_Extractor.SetFileQueueFromSelection(selection);
    ui->tableFileQueue->setModel(&FileListModel);

    QString task("Extracting ");
    task += QString::number(m_Extractor.NumFilesInQueue()) + " of " + archive.GetFileCount();
    task += " files from ";
    task += archive.GetFileName();
    ui->labelTask->setText(task);

    // Set initial path temporarily
    auto& appConfig = *Contexts::Get<ConfigFile>();
    const char* folder = appConfig["Folders"]["DefaultExtractFolder"].c_str();

    SetOutputFolder(folder);

    ui->buttonBox->buttons()[0]->setText("Extract");
}

FileExtractDialog::~FileExtractDialog()
{
    delete ui;
}

void FileExtractDialog::accept()
{
    if (bDone)
    {
        QDialog::accept();
    }
    else
    {
        ui->destinationFolderPath->setDisabled(true);
        ui->groupBox->setDisabled(true);
        ui->buttonBox->buttons()[1]->setDisabled(true);
        Start();
        ui->buttonBox->buttons()[0]->setText("Done");
        bDone = true;
    }
}

void FileExtractDialog::on_browseButton_clicked()
{
    const auto& folder = QFileDialog::getExistingDirectory(this, "Choose destination");
    SetOutputFolder(folder.toLatin1().data());
    auto& appConfig = *Contexts::Get<ConfigFile>();
    appConfig["Folders"]["DefaultExtractFolder"] = folder.toStdString();
}

void FileExtractDialog::Start()
{
    std::filesystem::path folder = ui->destinationFolderPath->text().toLatin1().data();
    if (!std::filesystem::exists(folder))
    {
        QMessageBox::warning(this, "Destination folder does not exist", "Would you like to create the folder now?");
        reject();
    }

    int numFiles = m_Extractor.NumFilesInQueue();
    int progress = 0;

    ui->progressBar->setRange(0, numFiles);

    std::thread worker(&FileExtractor::ExtractFiles, &m_Extractor, &progress, bOverwriteFiles);

    while (progress < numFiles)
    {
        ui->progressBar->setValue(progress);
        this->update();
    }

    ui->progressBar->setValue(numFiles);
    worker.join();
}

void FileExtractDialog::SetOutputFolder(const char* path)
{
    ui->destinationFolderPath->setText(path);
    std::filesystem::path p(path);
    p.make_preferred();
    m_Extractor.SetDestinationFolder(p);
}

void FileExtractDialog::on_checkBoxFilepaths_stateChanged(int arg1)
{
    bPreserveFolders = arg1 / 2;
}

void FileExtractDialog::on_checkBoxOverwrite_stateChanged(int arg1)
{
    bOverwriteFiles = arg1 / 2;
}
