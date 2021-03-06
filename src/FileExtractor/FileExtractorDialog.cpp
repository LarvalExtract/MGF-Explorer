#include "FileExtractorDialog.h"
#include "ui_FileExtractorDialog.h"

#include "mgf/Archive.h"

#include "Extractor.h"

#include "Utilities/configfile.h"
#include "Utilities/ContextProvider/ContextProvider.h"

#include <QFileDialog>

using namespace FileExtractor;

FileExtractorDialog::FileExtractorDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::FileExtractorDialog),
	Model(FileExtractList),
	StateMachine(this, &Model)
{
	setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	ui->setupUi(this);

	ui->tableFileQueue->setModel(&Model);

	const auto& path = (*ContextProvider::Get<ConfigFile>())["Folders"]["DefaultExtractFolder"];
	Destination = path;
	Destination.make_preferred();

	ui->destinationFolderPath->setText(path.c_str());

	StateMachine.ChangeState(States::EState::Idle);
}

FileExtractorDialog::~FileExtractorDialog()
{
	delete ui;
}

void FileExtractorDialog::SetSelection(const QModelIndexList& selection, const MGF::Archive& archive)
{
	FileExtractList = std::move(Extractor::ToList(selection));

	QString label = QString("Extracting %1 of %2 files from %3").arg(QString::number(FileExtractList.size()), QString::number(archive.GetFileCount()), archive.GetFileName());
	ui->labelTask->setText(label);
}

void FileExtractorDialog::on_browseButton_clicked()
{
	const auto& folder = QFileDialog::getExistingDirectory(this, "Choose destination").toStdString();
	
	Destination = folder;
	Destination.make_preferred();

	const auto& path = (*ContextProvider::Get<ConfigFile>())["Folders"]["DefaultExtractFolder"] = folder;
	ui->destinationFolderPath->setText(path.c_str());
}

void FileExtractorDialog::OnStarted()
{
	StateMachine.ChangeState(States::EState::Progress);
}

void FileExtractorDialog::OnFinished()
{
	StateMachine.ChangeState(States::EState::Finished);
}
