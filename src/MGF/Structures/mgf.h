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
	uint32_t unk1 = 0;
	uint32_t index = 0;
	uint32_t timestamp = 0;
	uint32_t checksum = 0;
	uint32_t hash = 0;
	uint32_t fileLength = 0;
	uint32_t fileOffset = 0;
};

struct MGF_FILE_RECORD_MA2 {
	uint32_t index = 0;
	uint32_t checksum = 0;
	uint32_t hash = 0;
	uint32_t fileLength = 0;
	uint32_t fileLength2 = 0;
	uint32_t timestamp = 0;
	uint32_t fileOffset = 0;
	uint32_t unknown = 0;
};

struct MGF_DIRECTORY {
	int32_t hash = 0;
	int32_t parentIndex = 0;
	int32_t childIndex = 0;
	int32_t siblingIndex = 0;
	int32_t stringOffset = 0;
	int32_t fileIndex = 0;
};
#pragma pack(pop)
