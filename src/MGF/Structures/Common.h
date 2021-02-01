#pragma once

#include <stdint.h>

struct MGI_HEADER
{
	uint32_t  unk1;
	uint8_t   MGIc[4];
	uint32_t  length;
	uint32_t  unk2;
	uint8_t   padding[16];
};