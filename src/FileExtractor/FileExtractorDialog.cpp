#include "FileExtractorDialog.h"
#include "ui_FileExtractorDialog.h"

#include "mgf/mgfarchive.h"

#include "Extractor.h"

#include "utils/configfile.h"
#include "utils/Contexts.h"

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

	const auto& path = (*Contexts::Get<ConfigFile>())["Folders"]["DefaultExtractFolder"];
	Destination = path;
	Destination.make_preferred();

	ui->destinationFolderPath->setText(path.c_str());

	StateMachine.ChangeState(States::EState::Idle);
}

FileExtractorDialog::~FileExtractorDialog()
{
	delete ui;
}

void FileExtractorDialog::SetSelection(const QModelIndexList& selection, const MGFArchive& archive)
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

	const auto& path = (*Contexts::Get<ConfigFile>())["Folders"]["DefaultExtractFolder"] = folder;
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
