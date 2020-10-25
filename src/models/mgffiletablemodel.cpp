#include "mgffiletablemodel.h"
#include "mgf/mgftreeitem.h"

enum class MGFFileTableModelColumn
{
    Index,
    GUID,
    Path,
    Offset,
    Length,
    Timestamp,

    Count
};

static const QVariant HEADERS[int(MGFFileTableModelColumn::Count)] = {
    "Index",
    "GUID",
    "File Path",
    "Offset",
    "Length",
    "Date"
};

int MGFFileTableModel::rowCount(const QModelIndex &parent) const
{
    return m_FileListReference.size();
}

int MGFFileTableModel::columnCount(const QModelIndex &parent) const
{
    return static_cast<int>(MGFFileTableModelColumn::Count);
}

QVariant MGFFileTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const MGFTreeItem& item = m_FileListReference[index.row()];
    switch(static_cast<MGFFileTableModelColumn>(index.column()))
    {
    case MGFFileTableModelColumn::Index:     return item.Index();
    case MGFFileTableModelColumn::GUID:      return QString::number(item.GUID(), 16);
    case MGFFileTableModelColumn::Path:      return QString(item.FilePath().string().c_str());
    case MGFFileTableModelColumn::Offset:    return item.FileOffset();
    case MGFFileTableModelColumn::Length:    return item.FileLength();
    case MGFFileTableModelColumn::Timestamp: return item.FileDate();
    }
}

QVariant MGFFileTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    return HEADERS[section];
}
