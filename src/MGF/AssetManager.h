#pragma once

#include "MGF/Assets/AssetBase.h"

#include <unordered_map>

namespace MGF {

	class AssetManager
	{
	public:
		MGF::Asset::AssetPtr Get(const MGF::File& file);

	private:
		std::unordered_map<std::uint64_t, MGF::Asset::AssetPtr> Assets;
	};

}