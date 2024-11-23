#include "MGFStringTable.h"

#include "MGF/Structures/MgText.h"
#include "MGF/Deserializer.h"

#include <vector>

MGFStringTable::MGFStringTable(const MGFFile& file) :
	MGFAsset(file)
{
	MGFFileDeserializer deserializer(file);

	const auto header = deserializer.Deserialize<MGTEXT_HEADER>();
	const auto buffer = deserializer.ReadAllBytes();

	Strings.resize(header.cText.numStrings);
	for (size_t i = 0; i < header.cText.numStrings; i++)
	{
		const auto [id, offset] = deserializer.Deserialize<MGTEXT_STRING_ENTRY>();

		Strings.emplace_back(id, QString::fromUtf16(reinterpret_cast<const char16_t*>(buffer.data() + offset)));
	}
}
