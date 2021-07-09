#include "AssetBase.h"
#include "PlainTextAsset.h"
#include "StringTableAsset.h"
#include "TextureAsset.h"
#include "ModelAsset.h"
#include "EntityAsset.h"

using namespace MGF::Asset;

AssetPtr AssetBase::Create(const File& soureFile)
{
	if (!soureFile.IsFile)
	{
		return nullptr;
	}

	using namespace Asset;
	switch (ToAssetType(soureFile.FileType))
	{
	case EAssetType::PlainText:   return std::make_shared<PlainTextAsset>(soureFile);
	case EAssetType::StringTable: return std::make_shared<StringTableAsset>(soureFile);
	case EAssetType::Texture:     return std::make_shared<TextureAsset>(soureFile);
	case EAssetType::Model:       return std::make_shared<ModelAsset>(soureFile);
	case EAssetType::Entity:      return std::make_shared<EntityAsset>(soureFile);
	}

	return nullptr;
}

EAssetType AssetBase::ToAssetType(const EFileType sourceFileType)
{
	switch (sourceFileType)
	{
	case EFileType::txt:		return EAssetType::PlainText;
	case EFileType::mgtext:		return EAssetType::StringTable;
	case EFileType::node:
	case EFileType::mgmodel:	return EAssetType::Model;
	case EFileType::tif:		return EAssetType::Texture;
	case EFileType::wdf:
	case EFileType::sdf:
	case EFileType::mtb:		return EAssetType::Entity;
	}

	return EAssetType::None;
}
