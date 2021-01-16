#pragma once

#include "EventData.h"

class MGF::File;

class EventSelectedItemChanged : public EventData
{
public:
	MGF::File* MgfItem = nullptr;
};