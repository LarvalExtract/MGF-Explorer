#pragma once

#include "State.h"

#include <vector>
#include <memory>

class StateGroup
{
	friend class State;

public:
	void ChangeState(int id);

protected:
	State* ActiveState = nullptr;
	std::vector<std::unique_ptr<State>> States;
};


