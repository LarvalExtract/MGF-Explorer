#include "Context.h"

int ContextBase::NumContexts = 0;

ContextProvider::ContextProvider()
{
	Contexts.resize(ContextBase::GetNumContexts(), nullptr);
}