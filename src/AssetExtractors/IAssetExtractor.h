#pragma once

#include <filesystem>

namespace MGF { class File; }

namespace AssetExtractor {

	class IAssetExtractor
	{
	public:
		virtual bool Extract(const MGF::File& sourceFile, const std::filesystem::path& destinationPath) = 0;
	};
}