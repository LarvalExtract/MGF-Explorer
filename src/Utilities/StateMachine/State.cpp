#include "State.h"
#include "StateGroup.h"

void State::GoTo(int stateID)
{
	States->ChangeState(stateID);
}
