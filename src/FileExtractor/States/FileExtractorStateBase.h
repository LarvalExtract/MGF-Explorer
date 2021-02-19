#pragma once

#include "Utilities/StateMachine/State.h"

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
			FileExtractorStateBase(StateGroup* stateGroup, FileExtractorDialog* dialog, Models::FileExtractListModel* model) :
				State(stateGroup),
				Dialog(dialog),
				Model(model) {}

		protected:
			FileExtractorDialog* Dialog;
			Models::FileExtractListModel* Model;
		};

	}
}
