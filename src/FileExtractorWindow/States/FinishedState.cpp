#include "FinishedState.h"
#include "FileExtractorStateGroup.h"

#include "FileExtractorWindow/FileExtractorDialog.h"
#include "FileExtractorWindow/ui_FileExtractorDialog.h"

using namespace FileExtractor::States;

void FinishedState::OnEnter()
{
	Dialog->ui->buttonBox->setEnabled(true);
	Dialog->ui->buttonBox->buttons()[0]->setText("Done");
	Dialog->ui->progressBar->setValue(Model->rowCount());

	Dialog->ui->browseButton->setEnabled(false);
	Dialog->ui->destinationFolderPath->setEnabled(false);
	Dialog->ui->groupBox->setEnabled(false);
}

void FinishedState::OnExit()
{

}
