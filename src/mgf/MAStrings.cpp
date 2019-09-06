#include "MAStrings.h"
#include "MGFArchive.h"

enum MGTextHeader
{
	DataSize = 64,
	StringCount = 68,
	FirstStringID = 72,
	LastStringID = 76,
	DataStart = 84
};

MAStrings::MAStrings(const MGFTreeNode& mgText)
{
	std::uint32_t size;
	std::uint32_t stringCount;
	mgText.archive.FileStream().seekg(mgText.FileOffset() + MGTextHeader::DataSize);
	mgText.archive.FileStream().read(reinterpret_cast<char*>(&size), 4);
	mgText.archive.FileStream().read(reinterpret_cast<char*>(&stringCount), 4);

	std::vector<char> buffer(size);
	mgText.archive.FileStream().seekg(mgText.FileOffset() + MGTextHeader::DataStart);
	mgText.archive.FileStream().read(buffer.data(), size);

	strings.reserve(stringCount);
	for (std::size_t i = 0; i < stringCount; i++)
	{
		long id = *reinterpret_cast<long*>(&buffer[i * 8]);

		int strOffset = *reinterpret_cast<int*>(&buffer[(i * 8) + 4]) - 16;
		wxString str(reinterpret_cast<const wchar_t*>(&buffer[strOffset]));

		strings.emplace_back(id, str);
	}
}

MAStrings::~MAStrings()
{

}

unsigned int MAStrings::GetColumnCount() const
{
	return 3;
}

wxString MAStrings::GetColumnType(unsigned int col) const
{
	switch (col)
	{
	case 0: return "string";
	case 1: return "string";
	case 2: return "string";
	}
}

unsigned int MAStrings::GetCount() const
{
	return strings.size();
}

void MAStrings::GetValueByRow(wxVariant& variant, unsigned row, unsigned col) const
{
	switch (col)
	{
	case 0: variant = wxString::Format("%i", row); break;
	case 1:	variant = wxString::Format("%i", strings[row].first); break;
	case 2: variant = strings[row].second; break;
	}
}

bool MAStrings::SetValueByRow(const wxVariant& variant, unsigned row, unsigned col)
{
	return false;
}
