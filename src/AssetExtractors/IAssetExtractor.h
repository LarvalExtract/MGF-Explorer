#pragma once

#include <filesystem>

class MGFFile;

namespace AssetExtractor {

	class IAssetExtractor
	{
	public:
		virtual bool Extract(const MGFFile& sourceFile, const std::filesystem::path& destinationPath) = 0;
	};
}