#pragma once

#include "Common.h"

#pragma pack(push, 1)
struct MGTEXT_HEADER
{
	MGI_HEADER cMgi;
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
	} cText;
};

struct MGTEXT_STRING_ENTRY {
	uint32_t id;
	uint32_t stringOffset;
};
#pragma pack(pop)