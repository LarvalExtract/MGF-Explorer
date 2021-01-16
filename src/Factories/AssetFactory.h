#pragma once

#include "mgf/AssetBase.h"

#include <memory>

class AssetFactory
{
public:
	static std::shared_ptr<MGF::Asset::AssetBase> Create(const MGF::File& file);

};

