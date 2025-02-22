#pragma once

#include "MGF/MGFFile.h"

#include <memory>

enum class MGFAssetType
{
	Model,
	Texture,
	PlainText,
	StringTable,
	Map,
	None
};

inline std::string_view ToString(MGFAssetType assetType)
{
	switch (assetType)
	{
	case MGFAssetType::Model:
		return "Model";

	case MGFAssetType::Texture:
		return "Texture";

	case MGFAssetType::PlainText:
		return "Text";

	case MGFAssetType::StringTable:
		return "String table";

	case MGFAssetType::Map:
		return "Map";

	case MGFAssetType::None:
		return "None";
	}
}

class MGFAsset
{
public:
	static std::shared_ptr<MGFAsset> Create(const MGFFile& mgfFile);
	static MGFAssetType MgfFileExtensionToAssetType(const std::filesystem::path& extension);

	const MGFFile& FileRef;
	const MGFAssetType AssetType = MGFAssetType::None;

protected:
	MGFAsset(const MGFFile& sourceFile);
};
