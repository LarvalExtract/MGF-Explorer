#include "ProgressState.h"
#include "FileExtractorStateGroup.h"

#include "FileExtractorWindow/FileExtractorDialog.h"
#include "FileExtractorWindow/ui_FileExtractorDialog.h"
#include "FileExtractorWindow/Extractor.h"

#include <future>
#include <chrono>

using namespace FileExtractor::States;

void ProgressState::OnEnter()
{
	Dialog->ui->destinationFolderPath->setEnabled(false);
	Dialog->ui->browseButton->setEnabled(false);
	Dialog->ui->groupBox->setEnabled(false);
	Dialog->ui->buttonBox->setEnabled(false);

	Dialog->ui->progressBar->setEnabled(true);
	Dialog->ui->progressBar->setRange(0, Model->rowCount());
	
	const auto result = std::async(
		std::launch::async,
		Extractor::ExtractFiles,
		std::ref(*Model),
		std::ref(Dialog->GetDestination()),
		Dialog->ui->checkBoxOverwrite->isChecked(),
		std::ref(ExtractedFileCount)
	);

	while (result.wait_for(std::chrono::milliseconds(20)) != std::future_status::ready)
	{
		Dialog->ui->progressBar->setValue(ExtractedFileCount);
		Dialog->ui->tableFileQueue->update();
	}

	Dialog->ui->progressBar->setValue(Model->rowCount());

	GoTo(EState::Finished);
}

void ProgressState::OnExit()
{
	Dialog->ui->destinationFolderPath->setEnabled(true);
	Dialog->ui->browseButton->setEnabled(true);
	Dialog->ui->groupBox->setEnabled(true);
	Dialog->ui->buttonBox->setEnabled(true);

	Dialog->ui->progressBar->setEnabled(false);
}
