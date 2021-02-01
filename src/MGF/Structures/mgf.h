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
	time_t   timestamp;
	uint64_t guid;
	uint32_t fileLength;
	uint32_t fileOffset;
};

struct MGF_FILE_RECORD_MA2 {
	uint32_t index;
	uint64_t guid;
	uint32_t fileLength;
	uint32_t fileLength2;
	time_t   timestamp;
	uint32_t fileOffset;
	char     padding[4];
};

struct MGF_DIRECTORY {
	int32_t unk1;
	int32_t parentIndex;
	int32_t folderIndex;
	int32_t unk2;
	int32_t stringOffset;
	int32_t fileIndex;
};
#pragma pack(pop)
