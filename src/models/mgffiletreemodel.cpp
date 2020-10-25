#include "mgffiletreemodel.h"
#include "mgf/mgftreeitem.h"

#include <QFileIconProvider>
#include <QLocale>

enum class MGFWorkspaceTreeView
{
    Column_Name,
    Column_Type,
    Column_Date,
    Column_Size,

    Column_Count
};

static const QVariant HEADERS[int(MGFWorkspaceTreeView::Column_Count)] = {
    "Name",
    "Type",
    "Date",
    "Size"
};

QVariant MGFFileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return HEADERS[section];
}

QModelIndex MGFFileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    auto parentItem = (parent.isValid()) ? static_cast<MGFTreeItem*>(parent.internalPointer()) : &m_FileListReference[0];
    auto childItem = parentItem->GetNthChild(row);

    return createIndex(row, column, const_cast<MGFTreeItem*>(childItem));
}

QModelIndex MGFFileTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    auto childItem = static_cast<const MGFTreeItem*>(child.internalPointer());
    auto parentItem = childItem->GetParent();

    if (parentItem == &m_FileListReference[0])
        return QModelIndex();

    return createIndex(parentItem->Row(), 0, const_cast<MGFTreeItem*>(parentItem));
}

int MGFFileTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    auto parentItem = (parent.isValid()) ? static_cast<const MGFTreeItem*>(parent.internalPointer()) : &m_FileListReference[0];

    return parentItem->GetChildCount();
}

int MGFFileTreeModel::columnCount(const QModelIndex &parent) const
{
    return static_cast<int>(MGFWorkspaceTreeView::Column_Count);
}

QVariant MGFFileTreeModel::data(const QModelIndex &index, int role) const
{
    static const QFileIconProvider IconProvider;
    static const QIcon Icons[2] =  {
        IconProvider.icon(QFileIconProvider::Folder),
        IconProvider.icon(QFileIconProvider::File)
    };
    static const QVariant empty;

    if (!index.isValid())
        return QVariant();

    auto item = static_cast<const MGFTreeItem*>(index.internalPointer());
    bool isFile = item->IsFile();

    if (role == Qt::DecorationRole && index.column() == 0)
        return Icons[isFile];

    QVariant type[2] = {empty, MGFTreeItem::FileTypeString(item->FileType())};
    QVariant date[2] = {empty, item->FileDate().toString("dd/MM/yyyy hh:mm")};
    QVariant size[2] = {empty, QLocale::system().formattedDataSize(item->FileLength(), 1)};

    if (role == Qt::DisplayRole)
    {
        switch (static_cast<MGFWorkspaceTreeView>(index.column()))
        {
        case MGFWorkspaceTreeView::Column_Name:  return item->Name();
        case MGFWorkspaceTreeView::Column_Type:  return type[isFile];
        case MGFWorkspaceTreeView::Column_Date:  return date[isFile];
        case MGFWorkspaceTreeView::Column_Size:  return size[isFile];
        }
    }

    return QVariant();
}
