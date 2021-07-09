#include "ArchiveFileTreeModel.h"
#include "MGF/Assets/AssetBase.h"

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
    return role == Qt::DisplayRole
        ? HEADERS[section]
        : QVariant();
}

QModelIndex ArchiveFileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const auto parentItem = parent.isValid() ? static_cast<MGF::File*>(parent.internalPointer()) : &FileListReference[0];
    const auto childItem = parentItem->GetNthChild(row);

    return createIndex(row, column, childItem);
}

QModelIndex ArchiveFileTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    const auto childItem = static_cast<const MGF::File*>(child.internalPointer());
    const auto parentItem = childItem->Parent;

    if (parentItem == &FileListReference[0])
        return QModelIndex();

    return createIndex(parentItem->TreeViewRow, 0, parentItem);
}

int ArchiveFileTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const auto parentItem = parent.isValid() ? static_cast<const MGF::File*>(parent.internalPointer()) : &FileListReference[0];

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
        return QVariant();

    const auto item = static_cast<const MGF::File*>(index.internalPointer());

    if (role == Qt::DecorationRole && index.column() == 0)
        return item->IsFile ? FileIcon : FolderIcon;

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
