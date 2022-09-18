#include "PlainTextAsset.h"
#include "MGF/Deserializer.h"

using namespace MGF::Asset;

PlainTextAsset::PlainTextAsset(const File& sourceFile) :
	AssetBase(sourceFile, EAssetType::PlainText),
	Text{[this](const File& file){
		const MGF::Deserializer deserializer(file);
		return deserializer.ReadAllBytes().data();
	}(sourceFile)}
{
	
}
