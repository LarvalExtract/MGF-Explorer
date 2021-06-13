#pragma once

#include "FileExtractorStateBase.h"

namespace FileExtractor { namespace States {

    class IdleState : public FileExtractorStateBase
	{
	public:
		IdleState(StateGroup* stateGroup, FileExtractorDialog* dialog, const std::unique_ptr<Models::FileExtractListModel>& model) :
			FileExtractorStateBase(stateGroup, dialog, model) {}

	protected:
		void OnEnter() override;
		void OnExit() override;

	};

}}
