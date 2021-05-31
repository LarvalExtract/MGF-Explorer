#include "AssetBase.h"
#include "PlainTextAsset.h"
#include "StringTableAsset.h"
#include "TextureAsset.h"
#include "ModelAsset.h"
#include "EntityAsset.h"

using namespace MGF::Asset;

AssetPtr AssetBase::Create(const File& soureFile)
{
	if (!soureFile.IsFile())
	{
		return nullptr;
	}

	using namespace Asset;
	switch (AssetBase::ToAssetType(soureFile.FileType()))
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
	case EFileType::Text:		return EAssetType::PlainText;
	case EFileType::MgText:		return EAssetType::StringTable;
	case EFileType::Node:
	case EFileType::MgModel:	return EAssetType::Model;
	case EFileType::Tif:		return EAssetType::Texture;
	case EFileType::Wdf:
	case EFileType::Mtb:		return EAssetType::Entity;
	}

	return EAssetType::None;
}