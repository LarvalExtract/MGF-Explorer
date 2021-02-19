#pragma once

#include <vector>

class ContextBase
{
public:
	static int GetNumContexts() { return NumContexts; }

protected:
	static int AllocContextIndex() { return NumContexts++; }

private:
	static int NumContexts;
};

template<class T>
class Context : public ContextBase
{
public:
	static int GetContextIndex() { return ContextIndex; }

private:
	static int ContextIndex;
};

template<class T>
int Context<T>::ContextIndex = ContextBase::AllocContextIndex();

class ContextProvider
{
public:
	ContextProvider();

	template<class T>
	T* GetContext() const
	{
		int index = Context<T>::GetContextIndex();
		return reinterpret_cast<T*>(Contexts[index]);
	}

	template<class T>
	void SetContext(T* ctx)
	{
		int index = Context<T>::GetContextIndex();
		Contexts[index] = reinterpret_cast<void*>(ctx);
	}

private:
	std::vector<void*> Contexts;
};