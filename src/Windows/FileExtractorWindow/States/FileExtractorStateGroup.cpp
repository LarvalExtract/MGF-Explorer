#include "FileExtractorStateGroup.h"

#include "FileExtractorWindow/FileExtractorDialog.h"
#include "FileExtractorWindow/Models/FileExtractListModel.h"

#include "IdleState.h"
#include "ProgressState.h"
#include "FinishedState.h"

using namespace FileExtractor::States;

FileExtractorStateGroup::FileExtractorStateGroup(FileExtractorDialog* dialog, const std::unique_ptr<Models::FileExtractListModel>& model)
{
	States.emplace_back(std::make_unique<IdleState>(this, dialog, model));
	States.emplace_back(std::make_unique<ProgressState>(this, dialog, model));
	States.emplace_back(std::make_unique<FinishedState>(this, dialog, model));
}

