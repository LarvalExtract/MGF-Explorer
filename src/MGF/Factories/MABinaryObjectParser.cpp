#include "MABinaryObjectParser.h"
#include "MAStrings.h"
#include "MGF/Deserializer.h"

using MagicNumber = uint32_t;
using Crc32 = uint32_t;

enum MABinaryAttributeDataTypeId : uint32_t
{
	Boolean = 1,
	Int32 = 3,
	Byte = 4,
	UInt16 = 5,
	Float = 7,
	String = 9,
	Float2 = 10,
	Float3 = 11,
	Colour = 12,
	Texture = 13
};

constexpr MagicNumber WDF_MAGIC_NUMBER = 0xFF002F00;
constexpr MagicNumber ENTITY_MAGIC_NUMBER = 0xFF012F01;
constexpr MagicNumber ATTRIBUTE_MAGIC_NUMBER = 0xFF022F02;

#pragma pack(push, 1)
struct OBJECT_ATTRIBUTE_HEADER
{
	MagicNumber magicNumber;
	MABinaryAttributeDataTypeId dataType;
	Crc32 crc;
	uint32_t dataLength;

};

struct OBJECT_HEADER
{
	MagicNumber magicNumber;
	uint32_t numAttributes;
	Crc32 crc;
};

struct OBJECT_FILE_HEADER
{
	MagicNumber magicNumber;
	uint32_t version;
	uint32_t numEntities;
	uint32_t highestEntityUID;
	uint32_t rootEntityUID;
};
#pragma pack(pop)

MABinaryObjectParser::MABinaryObjectParser(const MGFFile& InSourceFile, IListener* InListener)
	: SourceFile(InSourceFile)
	, Listener(InListener)
{
}

void MABinaryObjectParser::Parse()
{
	MGFFileDeserializer ObjectFileDeserializer(SourceFile);

	const OBJECT_FILE_HEADER FileHeader = ObjectFileDeserializer.Deserialize<OBJECT_FILE_HEADER>();
	if (FileHeader.magicNumber != WDF_MAGIC_NUMBER)
	{
		__debugbreak();
	}

	if (Listener)
	{
		Listener->OnHeaderRead(FileHeader.numEntities, FileHeader.rootEntityUID);
	}

	for (uint32_t EntityIndex = 0; EntityIndex < FileHeader.numEntities; ++EntityIndex)
	{
		const OBJECT_HEADER ObjectHeader = ObjectFileDeserializer.Deserialize<OBJECT_HEADER>();

		if (ObjectHeader.magicNumber != ENTITY_MAGIC_NUMBER)
		{
			__debugbreak();
		}

		MABinaryObject NewObject;
		NewObject.Name = MAStrings::GlobalStringCrcLookup(ObjectHeader.crc);
		NewObject.Attributes.reserve(ObjectHeader.numAttributes);

		for (uint32_t AttributeIndex = 0; AttributeIndex < ObjectHeader.numAttributes; ++AttributeIndex)
		{
			const OBJECT_ATTRIBUTE_HEADER AttributeHeader = ObjectFileDeserializer.Deserialize<OBJECT_ATTRIBUTE_HEADER>();

			if (AttributeHeader.magicNumber != ATTRIBUTE_MAGIC_NUMBER)
			{
				__debugbreak();
			}

			MABinaryObjectAttribute Attribute;
			Attribute.Name = MAStrings::GlobalStringCrcLookup(AttributeHeader.crc);
			Attribute.Length = AttributeHeader.dataLength;
			Attribute.Offset = ObjectFileDeserializer.CurrentOffset();

			switch (AttributeHeader.dataType)
			{
			case MABinaryAttributeDataTypeId::Boolean:
			case 16:
				Attribute.Value = ObjectFileDeserializer.Deserialize<bool>();
				break;

			case MABinaryAttributeDataTypeId::Int32:
			case 6:
				Attribute.Value = ObjectFileDeserializer.Deserialize<int32_t>();
				break;

			case MABinaryAttributeDataTypeId::Byte:
				Attribute.Value = ObjectFileDeserializer.Deserialize<uint8_t>();
				break;

			case MABinaryAttributeDataTypeId::UInt16:
				Attribute.Value = ObjectFileDeserializer.Deserialize<uint16_t>();
				break;

			case MABinaryAttributeDataTypeId::Colour:
				Attribute.Value = QColor(ObjectFileDeserializer.Deserialize<QRgb>());
				break;

			case MABinaryAttributeDataTypeId::Float:
				Attribute.Value = ObjectFileDeserializer.Deserialize<float>();
				break;

			case MABinaryAttributeDataTypeId::String:
			{
				std::string str(AttributeHeader.dataLength, 0);
				ObjectFileDeserializer.ReadBytes(&str[0], AttributeHeader.dataLength, ObjectFileDeserializer.CurrentOffset());
				ObjectFileDeserializer.CurrentOffset() += AttributeHeader.dataLength;
				Attribute.Value = str;
				break;
			}

			case MABinaryAttributeDataTypeId::Float2:
				Attribute.Value = QVector2D(ObjectFileDeserializer.Deserialize<float>(), ObjectFileDeserializer.Deserialize<float>());
				break;

			case MABinaryAttributeDataTypeId::Float3:
			case 14:
				Attribute.Value = QVector3D(ObjectFileDeserializer.Deserialize<float>(), ObjectFileDeserializer.Deserialize<float>(), ObjectFileDeserializer.Deserialize<float>());
				break;

			case MABinaryAttributeDataTypeId::Texture:
			{
				Attribute.Value = 0;
				ObjectFileDeserializer.CurrentOffset() += AttributeHeader.dataLength;
				break;
			}

			default:
				qDebug() << "MABinaryObjectParser: Unhandled data type" << AttributeHeader.dataType << " for attribute " << Attribute.Name.data();
				break;
			};

			if (Attribute.Name == UNKNOWN_STRING)
			{
				NewObject.UnknownAttributes.push_back(Attribute);
			}
			else
			{
				NewObject.Attributes.emplace(Attribute.Name, Attribute);
			}
		}

		if (Listener)
		{
			Listener->OnObjectRead(NewObject);
		}
	}
}
