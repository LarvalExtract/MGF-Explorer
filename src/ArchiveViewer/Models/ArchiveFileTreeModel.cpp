#include "ArchiveFileTreeModel.h"
#include "MGF/File.h"
#include "MGF/Assets/AssetMappings.h"

#include <QFileIconProvider>
#include <QLocale>

static const QVariant HEADERS[] = {
    "Name",
    "Type",
    "Date",
    "Size"
};

using namespace ArchiveViewer::Models;

QVariant ArchiveFileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return HEADERS[section];
}

QModelIndex ArchiveFileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    auto parentItem = (parent.isValid()) ? static_cast<MGF::File*>(parent.internalPointer()) : &FileListReference[0];
    auto childItem = parentItem->GetNthChild(row);

    return createIndex(row, column, const_cast<MGF::File*>(childItem));
}

QModelIndex ArchiveFileTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    auto childItem = static_cast<const MGF::File*>(child.internalPointer());
    auto parentItem = childItem->GetParent();

    if (parentItem == &FileListReference[0])
        return QModelIndex();

    return createIndex(parentItem->Row(), 0, const_cast<MGF::File*>(parentItem));
}

int ArchiveFileTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    auto parentItem = (parent.isValid()) ? static_cast<const MGF::File*>(parent.internalPointer()) : &FileListReference[0];

    return parentItem->GetChildCount();
}

int ArchiveFileTreeModel::columnCount(const QModelIndex &parent) const
{
    return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant ArchiveFileTreeModel::data(const QModelIndex &index, int role) const
{
    static const QFileIconProvider IconProvider;
    static const QIcon FileIcon = IconProvider.icon(QFileIconProvider::File);
    static const QIcon FolderIcon = IconProvider.icon(QFileIconProvider::Folder);

    auto AssetTypeString = [](MGF::EFileType type)
    {
		static const std::unordered_map<MGF::Asset::EAssetType, QVariant> assetTypeStrings = {
            { MGF::Asset::EAssetType::PlainText,   "Text" },
            { MGF::Asset::EAssetType::StringTable, "String Table" },
            { MGF::Asset::EAssetType::Texture,     "Texture" },
            { MGF::Asset::EAssetType::Model,       "Model" },
            { MGF::Asset::EAssetType::Entity,      "Map" },
            { MGF::Asset::EAssetType::None,        QVariant() }
		};

        auto assetType = MGF::Asset::sAssetMapping.at(type);
        return assetTypeStrings.at(assetType);
    };

    if (!index.isValid())
        return QVariant();

    auto item = static_cast<const MGF::File*>(index.internalPointer());
    bool isFile = item->IsFile();

    if (role == Qt::DecorationRole && index.column() == 0)
        return item->IsFile() ? FileIcon : FolderIcon;

    if (role == Qt::DisplayRole)
    {
        if (!item->IsFile())
        {
            return index.column() == 0 ? item->Name() : QVariant();
        }

        switch (index.column())
        {
		case 0:  return item->Name();
        case 1:  return AssetTypeString(item->FileType());
		case 2:  return item->FileDate().toString("dd/MM/yyyy hh:mm");
		case 3:  return QLocale::system().formattedDataSize(item->FileLength(), 1);
        }
    }

    return QVariant();
}
