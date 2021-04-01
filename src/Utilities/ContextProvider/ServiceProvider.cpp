#include "ServiceProvider.h"

ContextStore* ServiceProvider::pContextProvider = nullptr;

void ServiceProvider::Initialise()
{
	pContextProvider = new ContextStore();
}

void ServiceProvider::Destroy()
{
	delete pContextProvider;
}