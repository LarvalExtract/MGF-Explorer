#include "StringTable.h"

#include <vector>

struct MGTEXT {
	struct MGI {
		uint32_t  unk1;
		char      MGIc[4];
		uint32_t  length;
		uint32_t  unk2;
		char      padding[16];
	} mgChunkHeader;

	struct TEXT_HEADER {
		char      TEXT[4];
		char      padding[4];
		uint32_t  length;
		uint32_t  unk1;
		char      version[4];
		uint32_t  unk2;
		float     unk3;
		char	  data[4];
		uint32_t  stringChunkLength;
		uint32_t  numStrings = 0;
		uint32_t  firstID;
		uint32_t  lastID;
		uint32_t  unk4;
	} header;
};

struct MGTEXT_STRING_ENTRIES {
	uint32_t id;
	uint32_t stringOffset;
};

using namespace MGF::Asset;

StringTable::StringTable(const File& file) :
	AssetBase(file, MGFFileType::Strings)
{
	MGTEXT data;
	file.Read((char*)&data, 0, sizeof(data));

	std::vector<char> buffer(data.header.stringChunkLength);
	file.Read(buffer.data(), sizeof(data), buffer.size());
	
	Strings.reserve(data.header.numStrings);
	for (size_t i = 0; i < data.header.numStrings; i++)
	{
		MGTEXT_STRING_ENTRIES entry;
		entry = *reinterpret_cast<MGTEXT_STRING_ENTRIES*>(&buffer[i * sizeof(entry)]);
		
		uint32_t offset = entry.stringOffset + (sizeof(data) - 16);
		KeyStringPair pair;
		pair.key = entry.id;
		pair.str = QString::fromUtf16((char16_t*)&buffer[offset]);

		Strings.push_back(std::move(pair));
	}
}
