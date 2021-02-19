#pragma once

#include "Context.h"

class ContextProvider
{
public:
	ContextProvider() = default;
	ContextProvider(const ContextProvider&) = delete;
	ContextProvider(ContextProvider&&) = delete;
	ContextProvider& operator=(const ContextProvider&) = delete;
	ContextProvider& operator=(ContextProvider&&) = delete;

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

