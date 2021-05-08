#include "IdleState.h"
#include "FileExtractorStateGroup.h"

#include "FileExtractorWindow/FileExtractorDialog.h"
#include "FileExtractorWindow/ui_FileExtractorDialog.h"

#include <QObject>

using namespace FileExtractor::States;

void IdleState::OnEnter()
{
	Dialog->ui->destinationFolderPath->setEnabled(true);
	Dialog->ui->buttonBox->setEnabled(true);
	Dialog->ui->groupBox->setEnabled(true);

	Dialog->ui->buttonBox->buttons()[0]->setText("Extract");

	QObject::disconnect(Dialog->ui->buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
	QObject::connect(Dialog->ui->buttonBox, SIGNAL(accepted()), Dialog, SLOT(OnStarted()));
}

void IdleState::OnExit()
{
	Dialog->ui->destinationFolderPath->setEnabled(false);
	Dialog->ui->buttonBox->setEnabled(false);
	Dialog->ui->groupBox->setEnabled(false);

	QObject::disconnect(Dialog->ui->buttonBox, SIGNAL(accepted()), Dialog, SLOT(OnStarted()));
	QObject::connect(Dialog->ui->buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
}
