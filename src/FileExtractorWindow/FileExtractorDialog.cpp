#include "FileExtractorDialog.h"
#include "ui_FileExtractorDialog.h"

#include "MGF/Archive.h"

#include "Extractor.h"

#include "Utilities/configfile.h"
#include "Utilities/ContextProvider/ServiceProvider.h"

#include <QFileDialog>
#include <QMessageBox>

using namespace FileExtractor;

FileExtractorDialog::FileExtractorDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::FileExtractorDialog),
	StateMachine(this, Model)
{
	setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	ui->setupUi(this);

	setWindowTitle("Extract files");

	StateMachine.ChangeState(States::EState::Idle);
}

FileExtractorDialog::~FileExtractorDialog()
{
	delete ui;
}

void FileExtractorDialog::QueueFiles(const Models::FileItemList& fileList)
{
	Model.reset(new Models::FileExtractListModel(fileList));

	const auto& archive = Model->at(0).mgfItem->MGFArchive; 

	QString label = QString("Extracting %1 of %2 files from %3").arg(
		QString::number(Model->size()), 
		QString::number(archive.GetFileCount()), 
		archive.Path.filename().u8string().c_str()
	);

	ui->labelTask->setText(label);

	ui->tableFileQueue->setModel(Model.get());
}

void FileExtractorDialog::on_browseButton_clicked()
{
	const auto& folder = QFileDialog::getExistingDirectory(this, "Choose destination").toStdString();
	
	Destination = folder;
	Destination.make_preferred();

	ui->destinationFolderPath->setText(Destination.u8string().c_str());
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
