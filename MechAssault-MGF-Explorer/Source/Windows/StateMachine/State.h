#pragma once

class StateGroup;

class State
{
public:
	State(StateGroup* group) : States(group) {}

	void GoTo(int stateID);

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

private:
	StateGroup* States;
};