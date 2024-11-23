#include "MGFPlainText.h"
#include "MGF/Deserializer.h"


MGFPlainText::MGFPlainText(const MGFFile& sourceFile) :
	MGFAsset(sourceFile)
{
	const MGFFileDeserializer deserializer(sourceFile);
	Text = deserializer.ReadAllBytes().data();
}
