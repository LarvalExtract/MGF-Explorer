#pragma once

#include "MGF/MGFFile.h"

#include <QVector3D>
#include <QColor>

#include <QVariant>
#include <unordered_map>
#include <string_view>
#include <variant>

namespace MAStrings
{
	extern const char* GlobalStringCrcLookup(uint32_t CRC);
}

struct MABinaryObjectAttribute
{
	std::string_view Name;
	std::variant<bool, uint8_t, uint16_t, int32_t, QColor, float, QVector2D, QVector3D, std::string> Value;
	size_t Offset = 0;
	size_t Length = 0;
};

struct MABinaryObject
{
	std::string_view Name;
	std::unordered_map<std::string_view, MABinaryObjectAttribute> Attributes;
	std::vector<MABinaryObjectAttribute> UnknownAttributes;
};

class IListener
{
public:
	virtual void OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID) = 0;
	virtual void OnObjectRead(const MABinaryObject& BinaryObject) = 0;
};

// Deserializes various binary files in MechAssault used to store a list of objects. Each file type below uses the same format to serialize objects:
// * .wdf (maps)
// * .mtb 
class MABinaryObjectParser
{
public:
	MABinaryObjectParser(const MGFFile& InSourceFile, IListener* InListener = nullptr);

	void Parse();

protected:
	const MGFFile& SourceFile;
	IListener* Listener;
};