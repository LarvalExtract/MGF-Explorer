#include "FinishedState.h"
#include "FileExtractorStateGroup.h"

#include "Windows/FileExtractorWindow/FileExtractorDialog.h"
#include "Windows/FileExtractorWindow/ui_FileExtractorDialog.h"

#include "MGF/MGFArchive.h"

using namespace FileExtractor::States;

void FinishedState::OnEnter()
{
	Dialog->ui->buttonBox->setEnabled(true);
	Dialog->ui->buttonBox->buttons()[0]->setText("Done");
	Dialog->ui->buttonBox->removeButton(Dialog->ui->buttonBox->buttons()[1]);
	Dialog->ui->progressBar->setValue(Model->rowCount());

	Dialog->ui->browseButton->setEnabled(false);
	Dialog->ui->destinationFolderPath->setEnabled(false);
	Dialog->ui->groupBox->setEnabled(false);

	const unsigned int skipped = Model->rowCount() - ExtractedFileCount;

	const auto message = QString("Extracted %1 of %2 files from %3%4.").arg(
		QString::number(ExtractedFileCount),
		QString::number(Model->rowCount()),
		Model->at(0).mgfItem->MgfArchive.Path.filename().u8string().c_str(),
		skipped > 0 ? QString(" (%1 files were skipped because they already exist in the destination)").arg(QString::number(skipped)) : ""
		);

	Dialog->ui->labelProgress->setText(message);
}

void FinishedState::OnExit()
{

}
