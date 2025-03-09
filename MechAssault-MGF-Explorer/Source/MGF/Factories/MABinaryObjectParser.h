#pragma once

#include "MGF/MGFFile.h"
#include "Qt3DForwardDeclarations.h"

#include <QVector3D>
#include <QColor>

#include <unordered_map>
#include <variant>

namespace MAStrings
{
	extern const char* GlobalStringCrcLookup(uint32_t CRC);
}

struct MABinaryObjectAttribute
{
	MABinaryObjectAttribute() { Value = -1; }

	std::string_view Name;
	std::variant<bool, uint8_t, uint16_t, int32_t, QColor, float, QVector2D, QVector3D, std::string> Value;
	size_t Offset = 0;
	size_t Length = 0;

	operator bool() const { return get_impl<bool>(); }
	operator uint8_t() const { return get_impl<uint8_t>(); }
	operator uint16_t() const { return get_impl<uint16_t>(); }
	operator int32_t() const { return get_impl<int32_t>(); }
	operator QColor() const { return get_impl<QColor>(); }
	operator float() const { return get_impl<float>(); }
	operator QVector2D() const { return get_impl<QVector2D>(); }
	operator QVector3D() const { return get_impl<QVector3D>(); }
	operator std::string() const { return get_impl<std::string>(); }
	// operator Qt3DRender::QAbstractTexture* () const { return get_impl<Qt3DRender::QAbstractTexture*>(); }

	template<typename T> T get_impl() const { return std::holds_alternative<T>(Value) ? std::get<T>(Value) : T{}; }
};

struct MABinaryObject
{
	std::string_view Name;
	std::unordered_map<std::string_view, MABinaryObjectAttribute> Attributes;
	std::vector<MABinaryObjectAttribute> UnknownAttributes;

	MABinaryObjectAttribute operator[](std::string_view key) const { return Attributes.contains(key) ? Attributes.at(key) : MABinaryObjectAttribute(); }
};

class IListener
{
public:
	virtual void OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID) noexcept = 0;
	virtual void OnObjectRead(const MABinaryObject& BinaryObject) noexcept = 0;
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