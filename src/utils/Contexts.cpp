#include "Contexts.h"

ContextProvider* Contexts::pContextProvider = nullptr;

void Contexts::Initialise()
{
	pContextProvider = new ContextProvider();
}

void Contexts::Destroy()
{
	delete pContextProvider;
}