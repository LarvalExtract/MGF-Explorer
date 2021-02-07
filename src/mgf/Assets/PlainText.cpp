#include "PlainText.h"

MGF::Asset::PlainText::PlainText(const MGF::File& file) :
    AssetBase(file, EAssetType::PlainText)
{
	QByteArray data(file.FileLength() + 1, 0);

	file.Read(data.data());

	Text = data;
}
