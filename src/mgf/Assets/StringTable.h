#pragma once

#include "AssetBase.h"

#include <vector>

namespace MGF {	namespace Asset {

	class StringTable : public AssetBase
	{
	public:
		StringTable(const File& file);

		const auto& GetStrings() const { return Strings; }

	private:
		struct KeyStringPair {
			uint32_t key;
			QString str;
		};
		std::vector<KeyStringPair> Strings;
	};

} }


