#include "Context.h"

int ContextBase::NumContexts = 0;

ContextStore::ContextStore()
{
	Contexts.resize(ContextBase::GetNumContexts(), nullptr);
}