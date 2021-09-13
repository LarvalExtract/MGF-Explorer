#pragma once

#include <stdint.h>

#pragma pack(push, 1)
struct MGF_HEADER {
	char     signature[4];
	char     version;
	char     unk1;
	char     ZZ[2];
	uint32_t empty;
	uint32_t fileRecordCount;
	uint32_t fileRecordLength;
	uint32_t fileRecordOffset;
	uint32_t indexTableCount;
	uint32_t indexTableLength;
	uint32_t indexTableOffset;
	uint32_t stringsLength;
	uint32_t stringsOffset;
	char     padding[20];
};

struct MGF_FILE_RECORD_MA1 {
	uint32_t unk1;
	uint32_t index;
	uint32_t timestamp;
	uint32_t checksum;
	uint32_t hash;
	uint32_t fileLength;
	uint32_t fileOffset;
};

struct MGF_FILE_RECORD_MA2 {
	uint32_t index;
	uint32_t checksum;
	uint32_t hash;
	uint32_t fileLength;
	uint32_t fileLength2;
	uint32_t timestamp;
	uint32_t fileOffset;
	uint32_t unknown;
};

struct MGF_DIRECTORY {
	int32_t hash;
	int32_t parentIndex;
	int32_t childIndex;
	int32_t siblingIndex;
	int32_t stringOffset;
	int32_t fileIndex;
};
#pragma pack(pop)
