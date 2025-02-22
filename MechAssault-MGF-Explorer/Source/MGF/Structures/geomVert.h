#pragma once

#include "Common.h"

#pragma pack(push, 1)
struct VERS
{
	char8_t		id[4];
	uint32_t	pad1;
	uint32_t	chunk_size;
	uint32_t	pad2;
	struct INFO
	{
		char8_t		id[4];
		uint32_t	chunk_size;
		uint32_t	unk1;
		uint32_t	vertexCount;
		uint32_t	vertexBufferLayoutFlags;
		struct EXTENTS
		{
			struct float3 { float x, y, z; } min, max;
		} extents;
	} info;
	char8_t		data[4];
	uint32_t	data_size;
};

struct GEOMVERT_MA1
{
	MGI_HEADER	mgi;
	VERS		vers;
};

struct GEOMVERT_MA2
{
	MGI_HEADER	mgi;
	struct XMLB
	{
		char8_t		id[4];
		uint32_t	pad1;
		uint32_t	unk1;
		uint32_t	pad2;
		char8_t		vertbin[8];
	} xmlb;
	VERS		vers;
};
#pragma pack(pop)