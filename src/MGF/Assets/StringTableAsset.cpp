#include "StringTableAsset.h"

#include "MGF/Structures/MgText.h"

#include <vector>

using namespace MGF::Asset;

StringTableAsset::StringTableAsset(const File& file) :
	AssetBase(file, EAssetType::StringTable)
{
	const auto header = FileRef.Read<MGTEXT_HEADER>(0);

	std::vector<char> buffer(header.cText.stringChunkLength);
	FileRef.Read(buffer.data(), sizeof(header), buffer.size());

	Strings.resize(header.cText.numStrings);
	for (size_t i = 0; i < header.cText.numStrings; i++)
	{
		MGTEXT_STRING_ENTRY entry;
		entry = *reinterpret_cast<MGTEXT_STRING_ENTRY*>(&buffer[i * sizeof(entry)]);

		uint32_t offset = entry.stringOffset - 16;

		Strings.push_back(std::make_pair(
			entry.id, 
			QString::fromUtf16((char16_t*)&buffer[offset])
		));
	}
}
