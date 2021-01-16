#include "PlainText.h"

MGF::Asset::PlainText::PlainText(const MGF::File& file) :
    AssetBase(file, MGFFileType::Text)
{
	// Read data in to char buffer
	Text.reserve(file.FileLength());
	file.Read(&Text[0]);

	// Append null terminator
	Text.push_back(0);
}
