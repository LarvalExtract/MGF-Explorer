#pragma once

#include "EventData.h"

#include <vector>

class IEventListener;

class EventDispatcher
{
public:
	void Notify(EventData* data);

	void AddListener(IEventListener* subject);
	void RemoveListener(IEventListener* subject);

protected:
	std::vector<IEventListener*> Listeners;
};


