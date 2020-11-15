#include "StateGroup.h"

void StateGroup::ChangeState(int id)
{
	if (id >= 0 && id < States.size())
	{
		if (ActiveState)
			ActiveState->OnExit();

		ActiveState = States[id].get();
		ActiveState->OnEnter();
	}
}
