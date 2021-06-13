#include "ProgressState.h"
#include "FileExtractorStateGroup.h"

#include "FileExtractorWindow/FileExtractorDialog.h"
#include "FileExtractorWindow/ui_FileExtractorDialog.h"
#include "FileExtractorWindow/Extractor.h"

#include <future>

using namespace FileExtractor::States;

void ProgressState::OnEnter()
{
	Dialog->ui->destinationFolderPath->setEnabled(false);
	Dialog->ui->browseButton->setEnabled(false);
	Dialog->ui->groupBox->setEnabled(false);
	Dialog->ui->buttonBox->setEnabled(false);

	Dialog->ui->progressBar->setEnabled(true);
	Dialog->ui->progressBar->setRange(0, Model->rowCount());

	auto onFileExtracted = [this](int count)
	{
		QMetaObject::invokeMethod(Dialog->ui->progressBar, "setValue", Qt::QueuedConnection, Q_ARG(int, count));
		QMetaObject::invokeMethod(Dialog, "update", Qt::QueuedConnection);
	};

	auto onAllFiledExtracted = [this]()
	{
		QMetaObject::invokeMethod(Dialog, "OnFinished");
	};
	
	std::async(
		std::launch::async,
		Extractor::ExtractFiles,
		std::ref(*Model),
		std::ref(Dialog->GetDestination()),
		Dialog->ui->checkBoxOverwrite->isChecked(),
		onFileExtracted,
		onAllFiledExtracted
	);
}

void ProgressState::OnExit()
{
	Dialog->ui->destinationFolderPath->setEnabled(true);
	Dialog->ui->browseButton->setEnabled(true);
	Dialog->ui->groupBox->setEnabled(true);
	Dialog->ui->buttonBox->setEnabled(true);

	Dialog->ui->progressBar->setEnabled(false);
}
