#pragma once

#include "Context.h"

class ServiceProvider
{
public:
	ServiceProvider() = default;
	ServiceProvider(const ServiceProvider&) = delete;
	ServiceProvider(ServiceProvider&&) = delete;
	ServiceProvider& operator=(const ServiceProvider&) = delete;
	ServiceProvider& operator=(ServiceProvider&&) = delete;

	static void Initialise();
	static void Destroy();

	template<class T>
	static T* Inject()
	{
		T* ctx = pContextProvider->GetContext<T>();
		return ctx;
	}

	template<class T>
	static void Add(T* ctx)
	{
		pContextProvider->SetContext(ctx);
	}

private:
	static ContextStore* pContextProvider;
};

