#include "Archive.h"

#include "MGF/Structures/mgf.h"

#include <QFileIconProvider>
#include <QLocale>

#include "Assets/AssetBase.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type",
	"Date",
	"Size"
};

MGF::Archive::Archive(const std::filesystem::path& mgfFilePath)
	: std::ifstream(mgfFilePath, std::ios::binary),
	Path(mgfFilePath)
{
	if (!is_open())
	{
		throw std::runtime_error("Couldn't open " + mgfFilePath.u8string());
	}

	// read header from MGF file (first 64 bytes)
	MGF_HEADER mgfHeader;
	seekg(0, std::ios::beg);
	read(reinterpret_cast<char*>(&mgfHeader), sizeof(mgfHeader));

	// check for valid MGF file
	if (!(mgfHeader.signature[0] == 'm' &&
		mgfHeader.signature[1] == 'g' &&
		mgfHeader.signature[2] == 'f' &&
		mgfHeader.signature[3] == ' '))
	{
		throw std::runtime_error(mgfFilePath.u8string() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");
	}

	ArchiveVersion = static_cast<Version>(mgfHeader.version);
	FileCount = mgfHeader.fileRecordCount;

	std::vector<MGF_FILE_RECORD_MA1> fileRecordsMa1;
	std::vector<MGF_FILE_RECORD_MA2> fileRecordsMa2;

	if (ArchiveVersion == Version::MechAssault2LW)
	{
		fileRecordsMa2.resize(mgfHeader.fileRecordCount);
		seekg(mgfHeader.fileRecordOffset);
		read(reinterpret_cast<char*>(fileRecordsMa2.data()), mgfHeader.fileRecordLength);
	}
	else
	{
		fileRecordsMa1.resize(mgfHeader.fileRecordCount);
		seekg(mgfHeader.fileRecordOffset);
		read(reinterpret_cast<char*>(fileRecordsMa1.data()), mgfHeader.fileRecordLength);
	}

	std::vector<MGF_DIRECTORY> directoryRows(mgfHeader.indexTableCount);
	seekg(mgfHeader.indexTableOffset);
	read(reinterpret_cast<char*>(directoryRows.data()), mgfHeader.indexTableLength);

	std::vector<char> stringBuffer(mgfHeader.stringsLength);
	seekg(mgfHeader.stringsOffset);
	read(stringBuffer.data(), mgfHeader.stringsLength);

	seekg(0, std::ios::end);
	FileSize = tellg();

	Files.reserve(mgfHeader.indexTableCount);
	if (ArchiveVersion == Version::MechAssault)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const auto& row = directoryRows[i];
			const bool bIsFile = row.fileIndex != -1;

			Files.emplace_back(
				i,
				row.parentIndex,
				row.siblingIndex,
				row.childIndex,
				*this,
				&stringBuffer[row.stringOffset],
				bIsFile ? fileRecordsMa1[row.fileIndex].hash : row.hash,
				bIsFile ? fileRecordsMa1[row.fileIndex].checksum : 0,
				bIsFile ? fileRecordsMa1[row.fileIndex].fileOffset : 0,
				bIsFile ? fileRecordsMa1[row.fileIndex].fileLength : 0,
				bIsFile ? fileRecordsMa1[row.fileIndex].timestamp : 0,
				bIsFile
			);
		}
	}
	else if (ArchiveVersion == Version::MechAssault2LW)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const auto& row = directoryRows[i];
			const bool bIsFile = row.fileIndex != -1;

			Files.emplace_back(
				i,
				row.parentIndex,
				row.siblingIndex,
				row.childIndex,
				*this,
				&stringBuffer[row.stringOffset],
				bIsFile ? fileRecordsMa2[row.fileIndex].hash : row.hash,
				bIsFile ? fileRecordsMa2[row.fileIndex].checksum : 0,
				bIsFile ? fileRecordsMa2[row.fileIndex].fileOffset : 0,
				bIsFile ? fileRecordsMa2[row.fileIndex].fileLength : 0,
				bIsFile ? fileRecordsMa2[row.fileIndex].timestamp : 0,
				bIsFile
			);
		}
	}
	else
	{
		throw std::runtime_error("Unknown archive version");
	}
}

QModelIndex MGF::Archive::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	const auto parentItem = parent.isValid() ? static_cast<MGF::File*>(parent.internalPointer()) : &Files[0];
	const auto child = parentItem->ChildAt(row);

	return child ? createIndex(row, column, child) : QModelIndex();
}

QModelIndex MGF::Archive::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	const auto file = static_cast<MGF::File*>(child.internalPointer());
	const auto parent = file->Parent();
	
	return parent ? createIndex(file->_ParentIndex, 0, parent) : QModelIndex();
}

QModelIndex MGF::Archive::sibling(int row, int column, const QModelIndex& idx) const
{
	if (!hasIndex(row, column, idx) || !idx.isValid())
	{
		return QModelIndex();
	}

	const auto item = static_cast<MGF::File*>(idx.internalPointer());
	const auto sibling = item->SiblingAt(row);

	return sibling ? createIndex(row, column, sibling) : QModelIndex();
}

int MGF::Archive::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	const auto parentItem = parent.isValid() ? static_cast<MGF::File*>(parent.internalPointer()) : &Files[0];
	return parentItem->GetChildCount();
}

int MGF::Archive::columnCount(const QModelIndex& parent) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant MGF::Archive::data(const QModelIndex& index, int role) const
{
	static const QFileIconProvider IconProvider;
	static const QIcon FileIcon = IconProvider.icon(QFileIconProvider::File);
	static const QIcon FolderIcon = IconProvider.icon(QFileIconProvider::Folder);

	auto AssetTypeString = [](MGF::EFileType type)
	{
		switch (MGF::Asset::AssetBase::ToAssetType(type))
		{
		case MGF::Asset::EAssetType::PlainText:     return "Text";
		case MGF::Asset::EAssetType::StringTable:   return "String Table";
		case MGF::Asset::EAssetType::Texture:       return "Texture";
		case MGF::Asset::EAssetType::Model:         return "Model";
		case MGF::Asset::EAssetType::Entity:        return "Entities";
		}
		return "";
	};

	if (!index.isValid())
	{
		return QVariant();
	}

	const auto item = static_cast<const MGF::File*>(index.internalPointer());

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
		case 1:  return AssetTypeString(item->FileType);
		case 2:  return item->FileDate.toString("dd/MM/yyyy hh:mm");
		case 3:  return QLocale::system().formattedDataSize(item->FileLength, 1);
		}
	}

	return QVariant();
}

QVariant MGF::Archive::headerData(int section, Qt::Orientation orientation, int role) const
{
	return role == Qt::DisplayRole
		? HEADERS[section]
		: QVariant();
}