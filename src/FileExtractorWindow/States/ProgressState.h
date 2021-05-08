#pragma once

#include "FileExtractorStateBase.h"

namespace FileExtractor {	namespace States {

	class ProgressState : public FileExtractorStateBase
	{
	public:
		ProgressState(StateGroup* stateGroup, FileExtractorDialog* dialog, Models::FileExtractListModel* model) :
			FileExtractorStateBase(stateGroup, dialog, model) {}

	protected:
		void OnEnter() override;
		void OnExit() override;
	};

}}

