#include "AssetFactory.h"
#include "MGF/Assets/PlainText.h"
#include "MGF/Assets/Texture.h"
#include "MGF/Assets/StringTable.h"
#include "MGF/Assets/Model.h"

std::shared_ptr<MGF::Asset::AssetBase> AssetFactory::Create(const MGF::File& file)
{
	if (!file.IsFile())
	{
		return nullptr;
	}

	switch (file.FileType())
	{
	case MGF::FileType::Text:	return std::make_shared<MGF::Asset::PlainText>(file);
	case MGF::FileType::MgText:	return std::make_shared<MGF::Asset::StringTable>(file);
	case MGF::FileType::Tif:    return std::make_shared<MGF::Asset::Texture>(file);
	case MGF::FileType::Node:
	case MGF::FileType::MgModel: return std::make_shared<MGF::Asset::Model>(file);
	}

	return nullptr;
}
