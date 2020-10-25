#pragma once

#include "Context.h"

class Contexts
{
public:
	Contexts() = delete;
	Contexts(const Contexts&) = delete;
	Contexts(Contexts&&) = delete;
	Contexts& operator=(const Contexts&) = delete;
	Contexts& operator=(Contexts&&) = delete;

	static void Initialise();
	static void Destroy();

	template<class T>
	static T* Get()
	{
		T* ctx = pContextProvider->GetContext<T>();
		return ctx;
	}

	template<class T>
	static void Set(T* ctx)
	{
		pContextProvider->SetContext(ctx);
	}

private:
	static ContextProvider* pContextProvider;
};

