#include "MGFArchive.h"

#include <wx/datetime.h>

#include <Windows.h>
#include <stdint.h>

enum Shell32IconIndices : int
{
	GenericFile = 1,
	Folder = 5,
	TextFile = 152,
	CfgIniFile = 151
};

HINSTANCE hShellDLL = LoadLibraryA("shell32.dll");
HICON hFileIcon = static_cast<HICON>(LoadImageA(hShellDLL, MAKEINTRESOURCEA(Shell32IconIndices::GenericFile), IMAGE_ICON, 16, 16, LR_SHARED));
HICON hFolderIcon = static_cast<HICON>(LoadImageA(hShellDLL, MAKEINTRESOURCEA(Shell32IconIndices::Folder), IMAGE_ICON, 16, 16, LR_SHARED));
HICON hTxtFileIcon = static_cast<HICON>(LoadImageA(hShellDLL, MAKEINTRESOURCEA(Shell32IconIndices::TextFile), IMAGE_ICON, 16, 16, LR_SHARED));
HICON hCfgFileIcon = static_cast<HICON>(LoadImageA(hShellDLL, MAKEINTRESOURCEA(Shell32IconIndices::CfgIniFile), IMAGE_ICON, 16, 16, LR_SHARED));

extern wxString ToFileSizeStr(unsigned int bytes);

enum MGFArchiveHeaderOffset : int
{
	FileEntryCount = 12,
	FileEntryLength = 16,
	FileEntryOffset = 20,
	IndexEntryCount = 24,
	IndexEntryLength = 28,
	IndexEntryOffset = 32,
	StringsLength = 36,
	StringsOffset = 40
};

struct MA1FileEntry
{
	std::int32_t	unknown1;
	std::int32_t	ID;
	std::int32_t	timestamp;
	std::uint32_t	length2;
	std::int32_t	unknown2;
	std::uint32_t	length;
	std::uint32_t	offset;
};

struct MA2FileEntry
{
	std::int32_t	ID;
	std::int32_t	unknown1;
	std::int32_t	unknown2;
	std::uint32_t	length;
	std::uint32_t	length2;
	std::int32_t	timestamp;
	std::uint32_t	offset;
	std::uint32_t	padding;
};

struct MGFIndexEntry
{
	std::int32_t unknown1;
	std::int32_t parentIndex;
	std::int32_t unknown2;
	std::int32_t unknown3;
	std::int32_t stringOffset;
	std::int32_t isFile;
};

const std::uint32_t mgfSignature = 0x2066676D;

MGFArchive::MGFArchive(const std::filesystem::path& filepath) :
	fileStream(filepath, std::ios::binary),
	filename(filepath.filename().c_str())
{
	std::uint32_t header[16];
	fileStream.read(reinterpret_cast<char*>(&header[0]), 64);

	// Check the first 4 bytes are "mgf "
	if (header[0] != mgfSignature)
		throw;

	version = static_cast<MGFArchiveVersion>(header[1]);
	fileEntryCount = header[3];
	fileEntryLength = header[4];
	fileEntryOffset = header[5];
	indexEntryCount = header[6];
	indexEntryLength = header[7];
	indexEntryOffset = header[8];
	stringsLength = header[9];
	stringsOffset = header[10];

	InitTreeModel();

	genericFileIcon.CreateFromHICON(hFileIcon);
	folderIcon.CreateFromHICON(hFolderIcon);
	txtFileIcon.CreateFromHICON(hTxtFileIcon);
	cfgIniFileIcon.CreateFromHICON(hCfgFileIcon);

	fileStream.seekg(0, std::ios::end);
	size = fileStream.tellg();
}

MGFArchive::~MGFArchive()
{
	fileStream.close();
}

wxString MGFArchive::GetColumnType(unsigned int col) const
{
	return "string";
}

unsigned int MGFArchive::GetColumnCount() const
{
	return 4;
}

void MGFArchive::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
	if (MGFTreeNode* node = static_cast<MGFTreeNode*>(item.GetID()); node->IsFile())
	{
		wxDateTime date(static_cast<std::time_t>(node->LastModifiedDate()));

		switch (col)
		{
			// Provide the tree item name and a file/folder icon to the treeview
		case 0:
			switch (node->FileType())
			{
			case MGFFileType::PlainText_TXT: variant << wxDataViewIconText(node->Name(), txtFileIcon); break;
			case MGFFileType::PlainText_CFG_INI: variant << wxDataViewIconText(node->Name(), cfgIniFileIcon); break;
			default:	variant << wxDataViewIconText(node->Name(), genericFileIcon); break;
			}
			break;

			// Provide the time stamp
		case 1:
			variant = date.Format("%D %T");
			break;

			// Provide the file's type
		case 2:
			switch (node->FileType())
			{
			case MGFFileType::Texture:				variant = "Texture"; break;
			case MGFFileType::Model:				variant = "Model"; break;
			case MGFFileType::Strings:				variant = "Strings"; break;
			case MGFFileType::PlainText_TXT:		variant = "TXT"; break;
			case MGFFileType::PlainText_CFG_INI:	variant = "TXT"; break;
			case MGFFileType::None:					variant = wxEmptyString; break;
			}
			break;

			// Provide the file size
		case 3:
			variant = ToFileSizeStr(node->FileLength());
			break;
		}
	}

	else
	{
		switch (col)
		{
		case 0: variant << wxDataViewIconText(node->Name(), folderIcon); break;
		default: variant = wxEmptyString; break;
		}
	}
}

bool MGFArchive::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
	return false;
}

bool MGFArchive::IsEnabled(const wxDataViewItem& WXUNUSED(item), unsigned int WXUNUSED(col)) const
{
	return true;
}

wxDataViewItem MGFArchive::GetParent(const wxDataViewItem& item) const
{
	MGFTreeNode* node = static_cast<MGFTreeNode*>(item.GetID());

	if (node == &treeNodes[0])
		return wxDataViewItem(nullptr);

	return wxDataViewItem(node->GetParent());
}

bool MGFArchive::IsContainer(const wxDataViewItem& item) const
{
	MGFTreeNode* node = static_cast<MGFTreeNode*>(item.GetID());

	if (node == nullptr)
		return true;

	return !node->IsFile();
}

unsigned int MGFArchive::GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const
{
	if (MGFTreeNode* node = static_cast<MGFTreeNode*>(item.GetID()); node == nullptr)
	{
		children.reserve(treeNodes[0].GetChildCount());
		for (unsigned int i = 0; i < treeNodes[0].GetChildCount(); i++)
			children.Add(wxDataViewItem(treeNodes[0].GetNthChild(i)));

		return treeNodes[0].GetChildCount();
	}

	else
	{
		children.reserve(node->GetChildCount());
		for (unsigned int i = 0; i < node->GetChildCount(); i++)
			children.Add(wxDataViewItem(node->GetNthChild(i)));

		return node->GetChildCount();
	}
}

void MGFArchive::InitTreeModel()
{
	// Copy file entry data
	const auto fileEntryBuffer = [this]()
	{
		std::vector<char> result(fileEntryLength);
		fileStream.seekg(fileEntryOffset, std::ios::beg);
		fileStream.read(result.data(), fileEntryLength);

		return result;
	}();



	// Copy file index data
	const auto indexEntries = [this]()
	{
		std::vector<MGFIndexEntry> result(indexEntryCount);
		fileStream.seekg(this->indexEntryOffset, std::ios::beg);
		fileStream.read(reinterpret_cast<char*>(result.data()), indexEntryLength);

		return result;
	}();

	// Copy file and folder strings
	const auto stringBuffer = [this]()
	{
		std::vector<char> result(stringsLength);
		fileStream.seekg(stringsOffset, std::ios::beg);
		fileStream.read(result.data(), stringsLength);

		return result;
	}();

	treeNodes.reserve(indexEntryCount);
	treeNodes.emplace_back(nullptr, &stringBuffer[indexEntries[0].stringOffset], 0, 0, 0, 0, false, *this);

	for (std::size_t i = 1, j = 0; i < indexEntries.size(); i++)
	{
		// index entry is a folder
		if (indexEntries[i].isFile == -1)
		{
			treeNodes.emplace_back(
				&treeNodes[indexEntries[i].parentIndex],
				&stringBuffer[indexEntries[i].stringOffset],
				0,
				0,
				0,
				0,
				false,
				*this);
		}

		// index entry is a file
		else
		{
			switch (version)
			{
			case MGFArchiveVersion::MechAssault1:
				treeNodes.emplace_back(
					&treeNodes[indexEntries[i].parentIndex],
					&stringBuffer[indexEntries[i].stringOffset],
					reinterpret_cast<const MA1FileEntry*>(fileEntryBuffer.data())[j].ID,
					reinterpret_cast<const MA1FileEntry*>(fileEntryBuffer.data())[j].offset,
					reinterpret_cast<const MA1FileEntry*>(fileEntryBuffer.data())[j].length,
					reinterpret_cast<const MA1FileEntry*>(fileEntryBuffer.data())[j].timestamp,
					true,
					*this);
				break;

			case MGFArchiveVersion::MechAssault2LW:
				treeNodes.emplace_back(
					&treeNodes[indexEntries[i].parentIndex],
					&stringBuffer[indexEntries[i].stringOffset],
					reinterpret_cast<const MA2FileEntry*>(fileEntryBuffer.data())[j].ID,
					reinterpret_cast<const MA2FileEntry*>(fileEntryBuffer.data())[j].offset,
					reinterpret_cast<const MA2FileEntry*>(fileEntryBuffer.data())[j].length,
					reinterpret_cast<const MA2FileEntry*>(fileEntryBuffer.data())[j].timestamp,
					true,
					*this);
				break;
			}
			
			j++;
		}

		treeNodes[indexEntries[i].parentIndex].AddChild(&treeNodes[i]);
	}
}
