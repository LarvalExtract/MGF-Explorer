#include "ContextProvider.h"

ContextProvider* ContextProvider::pContextProvider = nullptr;

void ContextProvider::Initialise()
{
	pContextProvider = new ContextProvider();
}

void ContextProvider::Destroy()
{
	delete pContextProvider;
}