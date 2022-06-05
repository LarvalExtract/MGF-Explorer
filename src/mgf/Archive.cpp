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
		throw std::runtime_error("Couldn't open " + mgfFilePath.string());
	}

	// read header from MGF file (first 64 bytes)
	MGF_HEADER header{};
	seekg(0, std::ios::beg).read(reinterpret_cast<char*>(&header), sizeof(header));

	// check for valid MGF file
	if (!(header.signature[0] == 'm' &&
		header.signature[1] == 'g' &&
		header.signature[2] == 'f' &&
		header.signature[3] == ' '))
	{
		throw std::runtime_error(mgfFilePath.string() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");
	}

	ArchiveVersion = static_cast<Version>(header.version);
	FileCount = header.fileRecordCount;

	std::vector<MGF_FILE_RECORD_MA1> fileRecordsMa1;
	std::vector<MGF_FILE_RECORD_MA2> fileRecordsMa2;

	if (ArchiveVersion == Version::MechAssault2LW)
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
	if (ArchiveVersion == Version::MechAssault)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const auto& [dirhash, parentIndex, childIndex, siblingIndex, stringOffset, fileIndex] = directoryRows[i];
			const bool bIsFile = fileIndex != -1;
			const auto& [unk1, index, timestamp, checksum, filehash, fileLength, fileOffset] = bIsFile ? fileRecordsMa1[fileIndex] : MGF_FILE_RECORD_MA1{};

			Files.emplace_back(
				i,
				parentIndex,
				siblingIndex,
				childIndex,
				*this,
				&stringBuffer[stringOffset],
				bIsFile ? filehash : dirhash,
				bIsFile ? checksum : 0,
				bIsFile ? fileOffset : 0,
				bIsFile ? fileLength : 0,
				bIsFile ? timestamp : 0,
				bIsFile
			);
		}
	}
	else if (ArchiveVersion == Version::MechAssault2LW)
	{
		for (size_t i = 0; i < directoryRows.size(); i++)
		{
			const auto& [dirhash, parentIndex, childIndex, siblingIndex, stringOffset, fileIndex] = directoryRows[i];
			const bool bIsFile = fileIndex != -1;
			const auto [index, checksum, filehash, fileLength, fileLength2, timestamp, fileOffset, unknown] = bIsFile ? fileRecordsMa2[fileIndex] : MGF_FILE_RECORD_MA2{};

			Files.emplace_back(
				i,
				parentIndex,
				siblingIndex,
				childIndex,
				*this,
				&stringBuffer[stringOffset],
				bIsFile ? filehash : dirhash,
				checksum,
				fileOffset,
				fileLength,
				timestamp,
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
		return {};
	}
	
	const auto child = (parent.isValid() ? static_cast<MGF::File*>(parent.internalPointer()) : Root())->ChildAt(row);

	return child ? createIndex(row, column, child) : QModelIndex();
}

QModelIndex MGF::Archive::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return {};
	}

	const auto item = static_cast<MGF::File*>(child.internalPointer());
	
	return item->Parent() ? createIndex(item->_ParentIndex, 0, item->Parent()) : QModelIndex();
}

QModelIndex MGF::Archive::sibling(int row, int column, const QModelIndex& idx) const
{
	if (!hasIndex(row, column, idx) || !idx.isValid())
	{
		return {};
	}
	
	const auto sibling = static_cast<MGF::File*>(idx.internalPointer())->SiblingAt(row);

	return sibling ? createIndex(row, column, sibling) : QModelIndex();
}

int MGF::Archive::rowCount(const QModelIndex& parent) const
{
	return parent.column() == 0
		? (parent.isValid() ? static_cast<MGF::File*>(parent.internalPointer()) : Root())->GetChildCount()
		: 0;
}

int MGF::Archive::columnCount(const QModelIndex& parent) const
{
	return std::size(HEADERS);
}

QVariant MGF::Archive::data(const QModelIndex& index, int role) const
{
	static const QFileIconProvider IconProvider;
	static const QIcon FileIcon = IconProvider.icon(QFileIconProvider::File);
	static const QIcon FolderIcon = IconProvider.icon(QFileIconProvider::Folder);

	if (!index.isValid())
	{
		return {};
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
		case 1:
			switch (Asset::AssetBase::ToAssetType(item->FileType))
			{
			case Asset::EAssetType::PlainText:     return "Text";
			case Asset::EAssetType::StringTable:   return "String Table";
			case Asset::EAssetType::Texture:       return "Texture";
			case Asset::EAssetType::Model:         return "Model";
			case Asset::EAssetType::Entity:        return "Entities";
			}
			return "";
		case 2:  return item->FileDate.toString("dd/MM/yyyy hh:mm");
		case 3:  return QLocale::system().formattedDataSize(item->FileLength, 1);
		}
	}

	return {};
}

QVariant MGF::Archive::headerData(int section, Qt::Orientation orientation, int role) const
{
	return role == Qt::DisplayRole ? HEADERS[section]	: QVariant();
}