#include "PlainTextAsset.h"
#include "MGF/Deserializer.h"

using namespace MGF::Asset;

PlainTextAsset::PlainTextAsset(const File& sourceFile) :
	AssetBase(sourceFile, EAssetType::PlainText)
{
	const MGF::Deserializer deserializer(sourceFile);
	Text = deserializer.ReadAllBytes().data();
}
