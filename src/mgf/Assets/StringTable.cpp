#include "StringTable.h"

#include "MGF/Structures/MgText.h"

#include <vector>

using namespace MGF::Asset;

StringTable::StringTable(const File& file) :
	AssetBase(file, EAssetType::StringTable)
{
	MGTEXT_HEADER header;
	file.Read(reinterpret_cast<char*>(&header), 0, sizeof(header));

	std::vector<char> buffer(header.cText.stringChunkLength);
	file.Read(buffer.data(), sizeof(header), buffer.size());
	
	Strings.reserve(header.cText.numStrings);
	for (size_t i = 0; i < header.cText.numStrings; i++)
	{
		MGTEXT_STRING_ENTRY entry;
		entry = *reinterpret_cast<MGTEXT_STRING_ENTRY*>(&buffer[i * sizeof(entry)]);
		
		uint32_t offset = entry.stringOffset - 16;
		KeyStringPair pair;
		pair.key = entry.id;
		pair.str = QString::fromUtf16((char16_t*)&buffer[offset]);

		Strings.push_back(std::move(pair));
	}
}
