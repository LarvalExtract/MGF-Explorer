#pragma once

#include "Utilities/StateMachine/StateGroup.h"

namespace FileExtractor {

	class FileExtractorDialog;

	namespace Models {
		class FileExtractListModel;
	}

	namespace States {

		enum EState
		{
			Idle,
			Progress,
			Finished
		};

		class FileExtractorStateGroup : public StateGroup
		{
			friend class IdleState;
			friend class ProgressState;
			friend class FinishedState;

		public:
			FileExtractorStateGroup(FileExtractorDialog* dialog, Models::FileExtractListModel* model);
		};

}}