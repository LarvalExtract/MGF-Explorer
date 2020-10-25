#ifndef MGFFILETABLEMODEL_H
#define MGFFILETABLEMODEL_H

#include <QAbstractTableModel>

class MGFTreeItem;

class MGFFileTableModel : public QAbstractTableModel
{
public:
    MGFFileTableModel(const std::vector<MGFTreeItem>& mgfItems) : m_FileListReference(mgfItems) {}

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    const std::vector<MGFTreeItem>& m_FileListReference;
};

#endif // MGFFILETABLEMODEL_H
