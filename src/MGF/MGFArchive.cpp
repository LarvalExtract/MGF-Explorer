#include "MGFArchive.h"
#include "MGF/Structures/mgf.h"
#include "MGFExplorerApplication.h"
#include "Assets/MGFAsset.h"

#include <QFileIconProvider>
#include <QLocale>

static const QVariant HEADERS[] = {
	"Name",
	"Type",
	"Date",
	"Size"
};

MGFArchive::MGFArchive(const std::filesystem::path& mgfFilePath)
	: std::ifstream(mgfFilePath, std::ios::binary),
	Path(mgfFilePath)
{
	if (!is_open())
	{
		throw std::runtime_error("Couldn't open " + mgfFilePath.string());
	}

	// read header from MGF file (first 64 bytes)
	MGF_HEADER header{};
	seekg(0, std::ios::beg).read(reinterpret_cast<char*>(&header), sizeof(header));

	static constexpr uint32_t mgf_signature = 0x2066676D;

	// check for valid MGF file
	if (header.signature != mgf_signature)
	{
		throw std::runtime_error(mgfFilePath.string() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");
	}

	ArchiveVersion = static_cast<MGFArchiveVersion>(header.version);
	FileCount = header.fileRecordCount;

	std::vector<MGF_FILE_RECORD_MA1> fileRecordsMa1;
	std::vector<MGF_FILE_RECORD_MA2> fileRecordsMa2;

	if (ArchiveVersion == MGFArchiveVersion::MechAssault2LW)
	{
		fileRecordsMa2.resize(header.fileRecordCount);
		seekg(header.fileRecordOffset).read(reinterpret_cast<char*>(fileRecordsMa2.data()), header.fileRecordLength);
	}
	else
	{
		fileRecordsMa1.resize(header.fileRecordCount);
		seekg(header.fileRecordOffset).read(reinterpret_cast<char*>(fileRecordsMa1.data()), header.fileRecordLength);
	}

	std::vector<MGF_DIRECTORY> directoryRows(header.indexTableCount);
	seekg(header.indexTableOffset).read(reinterpret_cast<char*>(directoryRows.data()), header.indexTableLength);

	std::vector<char> stringBuffer(header.stringsLength);
	seekg(header.stringsOffset).read(stringBuffer.data(), header.stringsLength);

	Size = seekg(0, std::ios::end).tellg();

	Files.reserve(header.indexTableCount);
	if (ArchiveVersion == MGFArchiveVersion::MechAssault)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const MGF_DIRECTORY directoryEntry = directoryRows[i];
			const bool bIsFile = directoryEntry.fileIndex != -1;
			const MGF_FILE_RECORD_MA1 fileRecord = bIsFile ? fileRecordsMa1[directoryEntry.fileIndex] : MGF_FILE_RECORD_MA1{};

			Files.emplace_back(
				*this,
				&stringBuffer[directoryEntry.stringOffset],
				bIsFile ? fileRecord.hash : directoryEntry.hash,
				fileRecord.checksum,
				fileRecord.fileOffset,
				fileRecord.fileLength,
				fileRecord.timestamp,
				bIsFile
			);
		}
	}
	else if (ArchiveVersion == MGFArchiveVersion::MechAssault2LW)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const MGF_DIRECTORY directoryEntry = directoryRows[i];
			const bool bIsFile = directoryEntry.fileIndex != -1;
			const MGF_FILE_RECORD_MA2 fileRecord = bIsFile ? fileRecordsMa2[directoryEntry.fileIndex] : MGF_FILE_RECORD_MA2{};

			Files.emplace_back(
				*this,
				&stringBuffer[directoryEntry.stringOffset],
				bIsFile ? fileRecord.hash : directoryEntry.hash,
				fileRecord.checksum,
				fileRecord.fileOffset,
				fileRecord.fileLength,
				fileRecord.timestamp,
				bIsFile
			);
		}
	}
	else
	{
		throw std::runtime_error("Unknown archive version");
	}

	// Second pass
	for (size_t i = 0; i < directoryRows.size(); ++i)
	{
		if (const auto directoryEntry = directoryRows[i]; directoryEntry.parentIndex >= 0)
		{
			Files.at(directoryEntry.parentIndex).AddChild(&Files.at(i));
		}
	}
}

QModelIndex MGFArchive::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return {};
	}
	
	const auto child = (parent.isValid() ? static_cast<MGFFile*>(parent.internalPointer()) : Root())->ChildAt(row);

	return child ? createIndex(row, column, child) : QModelIndex();
}

QModelIndex MGFArchive::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return {};
	}

	const auto item = static_cast<MGFFile*>(child.internalPointer());
	
	return item->Parent() ? createIndex(item->Parent()->Row(), 0, item->Parent()) : QModelIndex();
}

int MGFArchive::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid())
	{
		const int childCount = parent.column() == 0
			? static_cast<MGFFile*>(parent.internalPointer())->GetChildCount()
			: 0;
		return childCount;
	}
	
	return Root()->GetChildCount();
}

int MGFArchive::columnCount(const QModelIndex& parent) const
{
	return std::size(HEADERS);
}

QVariant MGFArchive::data(const QModelIndex& index, int role) const
{
	static const QFileIconProvider IconProvider;
	static const QIcon FileIcon = IconProvider.icon(QFileIconProvider::File);
	static const QIcon FolderIcon = IconProvider.icon(QFileIconProvider::Folder);

	if (!index.isValid())
	{
		return {};
	}

	const auto item = static_cast<const MGFFile*>(index.internalPointer());

	if (role == Qt::DecorationRole && index.column() == 0)
	{
		return item->IsFile ? FileIcon : FolderIcon;
	}

	if (role == Qt::DisplayRole)
	{
		if (!item->IsFile)
		{
			return index.column() == 0 ? item->Name : QVariant();
		}

		switch (index.column())
		{
		case 0:  return item->Name;
		case 1:  return ToString(MGFAsset::MgfFileExtensionToAssetType(item->FilePath().extension())).data();
		case 2:  return item->FileDate.toString("dd/MM/yyyy hh:mm");
		case 3:  return QLocale::system().formattedDataSize(item->FileLength, 1);
		}
	}

	return {};
}

QVariant MGFArchive::headerData(int section, Qt::Orientation orientation, int role) const
{
	return role == Qt::DisplayRole ? HEADERS[section]	: QVariant();
}

std::shared_ptr<MGFAsset> MGFArchive::LoadAsset(const MGFFile& mgfFile)
{
	return qApp->GetAsset(mgfFile);
}
