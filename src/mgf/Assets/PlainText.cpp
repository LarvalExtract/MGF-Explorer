#include "PlainText.h"

using namespace MGF::Asset;

PlainText::PlainText(const File& soureFile) :
	AssetBase(soureFile, EAssetType::PlainText)
{
	const auto length = soureFile.FileLength() + 1;

	QByteArray data(length, 0);
	soureFile.Read(data.data());

	Text = data;
}
