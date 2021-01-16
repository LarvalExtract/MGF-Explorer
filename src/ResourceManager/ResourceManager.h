#pragma once

#include "mgf/AssetBase.h"
#include "Factories/AssetFactory.h"

#include <unordered_map>
#include <memory>

namespace MGF {
	using AssetPtr = std::shared_ptr<MGF::Asset::AssetBase>;
}

class ResourceManager
{
public:
	MGF::AssetPtr Get(const MGF::File& file);

private:
	std::unordered_map<std::uint64_t, MGF::AssetPtr> Assets;
	AssetFactory Factory;
};