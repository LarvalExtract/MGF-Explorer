#pragma once

#include "utils/events/IEventListener.h"

class StateGroup;

class State : public IEventListener
{
public:
	State(StateGroup* group) : States(group) {}

	void GoTo(int stateID);

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

private:
	StateGroup* States;
};