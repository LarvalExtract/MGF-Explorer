#pragma once

#include "AssetExtractors/IAssetExtractor.h"

#include <QImageWriter>

namespace AssetExtractor {

	class TextureExtractor : public IAssetExtractor
	{
	public:
		bool Extract(const MGFFile& sourceFile, const std::filesystem::path& destinationPath) override;

	private:
		QImageWriter ImageWriter;
	};

}