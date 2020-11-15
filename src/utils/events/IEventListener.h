#pragma once

#include "EventData.h"

class IEventListener
{
public:
	virtual void OnNotify(EventData* data) {}
};