#include "ContextProvider.h"

ContextStore* ContextProvider::pContextProvider = nullptr;

void ContextProvider::Initialise()
{
	pContextProvider = new ContextStore();
}

void ContextProvider::Destroy()
{
	delete pContextProvider;
}