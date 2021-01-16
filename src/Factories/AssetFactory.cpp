#include "AssetFactory.h"
#include "mgf/Assets/PlainText.h"

std::shared_ptr<MGF::Asset::AssetBase> AssetFactory::Create(const MGF::File& file)
{
	switch (file.FileType())
	{
	case MGFFileType::Text:		return std::make_shared<MGF::Asset::PlainText>(file);
	case MGFFileType::Strings:	break;
	}

	return nullptr;
}
