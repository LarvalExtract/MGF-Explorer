#pragma once

#include "Utilities/StateMachine/State.h"

#include <memory>

namespace FileExtractor 
{ 
	class FileExtractorDialog;
	
	namespace Models {
		class FileExtractListModel;
	}

	namespace States {

        class FileExtractorStateBase : public State
		{
		public:
			FileExtractorStateBase(StateGroup* stateGroup, FileExtractorDialog* dialog, const std::unique_ptr<Models::FileExtractListModel>& model) :
				State(stateGroup),
				Dialog(dialog),
				Model(model) {}

		protected:
			FileExtractorDialog* Dialog;
			const std::unique_ptr<Models::FileExtractListModel>& Model;
			unsigned int ExtractedFileCount = 0;
		};

	}
}
