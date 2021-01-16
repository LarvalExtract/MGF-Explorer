#pragma once

#include "mgf/AssetBase.h"

#include <vector>

namespace MGF {	namespace Asset {

	class StringTable : public AssetBase
	{
	public:
		StringTable(const File& file);

	private:
		struct KeyStringPair {
			uint32_t key;
			QString str;
		};
		std::vector<KeyStringPair> Strings;
	};

} }


