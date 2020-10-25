#include "EventDispatcher.h"
#include "IEventListener.h"

#include <algorithm>

void EventDispatcher::Notify(EventData* data)
{
	for (const auto subject : Listeners)
	{
		subject->OnNotify(data);
	}
}

void EventDispatcher::AddListener(IEventListener* subject)
{
	Listeners.push_back(subject);
}

void EventDispatcher::RemoveListener(IEventListener* subject)
{
	Listeners.erase(std::remove(Listeners.begin(), Listeners.end(), subject));
}