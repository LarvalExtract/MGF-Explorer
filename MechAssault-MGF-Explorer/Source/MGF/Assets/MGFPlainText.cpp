#include "MGFPlainText.h"
#include "MGF/Deserializer.h"


MGFPlainText::MGFPlainText(const MGFFile& sourceFile) :
	MGFAsset(sourceFile)
{
	const MGFFileDeserializer deserializer(sourceFile);
	const std::vector<char> bytes = deserializer.ReadAllBytes();
	Text = QString::fromLatin1(bytes.data(), bytes.size());
}
