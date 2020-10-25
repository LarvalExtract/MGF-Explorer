#pragma once

#include "EventData.h"

class MGFTreeItem;

class EventSelectedItemChanged : public EventData
{
public:
	MGFTreeItem* MgfItem = nullptr;
};