#include "MGFAsset.h"
#include "MGFModel.h"
#include "MGFTexture.h"
#include "MGFPlainText.h"
#include "MGFStringTable.h"
#include "MGFMap.h"

std::shared_ptr<MGFAsset> MGFAsset::Create(const MGFFile& mgfFile)
{
	const std::filesystem::path ext = mgfFile.FilePath().extension();

	switch (MgfFileExtensionToAssetType(ext))
	{
	case MGFAssetType::Model:
		return std::make_shared<MGFModel>(mgfFile);

	case MGFAssetType::Texture:
		return std::make_shared<MGFTexture>(mgfFile);

	case MGFAssetType::PlainText:
		return std::make_shared<MGFPlainText>(mgfFile);

	case MGFAssetType::StringTable:
		return std::make_shared<MGFStringTable>(mgfFile);

	case MGFAssetType::Map:
		return std::make_shared<MGFMap>(mgfFile);
	}

	return nullptr;
}

MGFAssetType MGFAsset::MgfFileExtensionToAssetType(const std::filesystem::path& extension)
{
	std::string ext = extension.string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

	if (ext == ".mgmodel" ||
		ext == ".node")
	{
		return MGFAssetType::Model;
	}

	if (ext == ".tif")
	{
		return MGFAssetType::Texture;
	}

	if (ext == ".ini" ||
		ext == ".cfg" ||
		ext == ".txt")
	{
		return MGFAssetType::PlainText;
	}

	if (ext == ".mgtext")
	{
		return MGFAssetType::StringTable;
	}

	if (ext == ".wdf")
	{
		return MGFAssetType::Map;
	}

	return MGFAssetType::None;
}

MGFAsset::MGFAsset(const MGFFile& sourceFile)
	: FileRef(sourceFile)
	, AssetType(MgfFileExtensionToAssetType(sourceFile.FilePath().extension()))
{
}
