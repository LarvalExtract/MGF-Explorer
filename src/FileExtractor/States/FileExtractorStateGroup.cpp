#include "FileExtractorStateGroup.h"

#include "FileExtractor/FileExtractorDialog.h"
#include "FileExtractor/Models/FileExtractListModel.h"

#include "IdleState.h"
#include "ProgressState.h"
#include "FinishedState.h"

using namespace FileExtractor::States;

FileExtractorStateGroup::FileExtractorStateGroup(FileExtractorDialog* dialog, Models::FileExtractListModel* model)
{
	States.emplace_back(std::make_unique<IdleState>(this, dialog, model));
	States.emplace_back(std::make_unique<ProgressState>(this, dialog, model));
	States.emplace_back(std::make_unique<FinishedState>(this, dialog, model));
}

