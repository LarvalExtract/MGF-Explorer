#include "AssetFactory.h"
#include "MGF/Assets/AssetMappings.h"
#include "MGF/Assets/PlainText.h"
#include "MGF/Assets/Texture.h"
#include "MGF/Assets/StringTable.h"
#include "MGF/Assets/ModelAsset.h"
#include "MGF/Assets/EntityAsset.h"

std::shared_ptr<MGF::Asset::AssetBase> AssetFactory::Create(const MGF::File& file)
{
	if (!file.IsFile())
	{
		return nullptr;
	}

	switch (MGF::Asset::sAssetMapping.at(file.FileType()))
	{
	case MGF::Asset::EAssetType::PlainText:   return std::make_shared<MGF::Asset::PlainText>(file);
	case MGF::Asset::EAssetType::StringTable: return std::make_shared<MGF::Asset::StringTable>(file);
	case MGF::Asset::EAssetType::Texture:     return std::make_shared<MGF::Asset::Texture>(file);
	case MGF::Asset::EAssetType::Model:       return std::make_shared<MGF::Asset::ModelAsset>(file);
	case MGF::Asset::EAssetType::Entity:      return std::make_shared<MGF::Asset::EntityAsset>(file);
	}

	return nullptr;
}
