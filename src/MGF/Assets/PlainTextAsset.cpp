#include "PlainTextAsset.h"

using namespace MGF::Asset;

PlainTextAsset::PlainTextAsset(const File& soureFile) :
	AssetBase(soureFile, EAssetType::PlainText),
	Text{[this](){
		const auto length = FileRef.FileLength + 1;
		QByteArray data(length, 0);
		FileRef.Read(data.data());
		return data;
	}()}
{
	
}
