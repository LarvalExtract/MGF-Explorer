#include "FileExtractorDialog.h"
#include "ui_FileExtractorDialog.h"

#include "MGF/MGFArchive.h"
#include "Extractor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

using namespace FileExtractor;

static const QString s_SavedFileExtractorDestinationKey = "SavedFileExtractorDestinationPath";

FileExtractorDialog::FileExtractorDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::FileExtractorDialog),
	StateMachine(this, Model)
{
	ui->setupUi(this);

	setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	setWindowTitle("Extract files");

	const QSettings settings;
	ui->destinationFolderPath->setText(settings.value(s_SavedFileExtractorDestinationKey).toString());

	StateMachine.ChangeState(States::EState::Idle);
}

FileExtractorDialog::~FileExtractorDialog()
{
	delete ui;
}

void FileExtractorDialog::QueueFiles(const Models::FileItemList& fileList)
{
	Model.reset(new Models::FileExtractListModel(fileList));

	const auto& archive = Model->at(0).mgfItem->MgfArchive; 

	const auto label = QString("Extracting %1 of %2 files from %3").arg(
		QString::number(Model->size()), 
		QString::number(archive.GetFileCount()), 
		archive.Path.filename().string().c_str()
	);

	ui->labelTask->setText(label);
	ui->tableFileQueue->setModel(Model.get());
}

void FileExtractorDialog::on_browseButton_clicked()
{
	const auto& folder = QFileDialog::getExistingDirectory(this, "Choose destination").toStdString();
	
	Destination = folder;
	Destination.make_preferred();

	QSettings settings;
	settings.setValue(s_SavedFileExtractorDestinationKey, Destination.string().c_str());
	ui->destinationFolderPath->setText(settings.value(s_SavedFileExtractorDestinationKey).toString());
}

void FileExtractorDialog::OnStarted()
{
	if (!std::filesystem::exists(Destination))
	{
		QMessageBox::critical(this, "Error", "Destination folder does not exist");
		return;
	}

	StateMachine.ChangeState(States::EState::Progress);
}

void FileExtractorDialog::OnFinished()
{
	StateMachine.ChangeState(States::EState::Finished);
}
