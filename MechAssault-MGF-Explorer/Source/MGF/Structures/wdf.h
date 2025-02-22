#pragma once

#include <stdint.h>

constexpr uint32_t WDF_MAGIC_NUMBER			= 0x002F00FF;
constexpr uint32_t ENTITY_MAGIC_NUMBER		= 0x012F01FF;
constexpr uint32_t ATTRIBUTE_MAGIC_NUMBER	= 0x022F02FF;

#pragma pack(push, 1)
struct OBJECT_ATTRIBUTE_HEADER
{
	uint32_t magicNumber;
	uint32_t dataType;
	uint32_t crc;
	uint32_t dataLength;

};

struct OBJECT_HEADER
{
	uint32_t magicNumber;
	uint32_t numAttributes;
	uint32_t crc;

};

struct OBJECT_FILE_HEADER
{
	uint32_t magicNumber;
	uint32_t version;
	uint32_t numEntities;
	uint32_t highestEntityUID;
	uint32_t rootEntityUID;

};
#pragma pop