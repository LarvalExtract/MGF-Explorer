#pragma once

#include <stdint.h>

namespace MAStrings
{
	#define UNKNOWN_STRING "???"

	const char* GlobalStringCrcLookup(uint32_t Crc);
}