#ifndef MGFFILETREEMODEL_H
#define MGFFILETREEMODEL_H

#include <QAbstractItemModel>

namespace MGF {
    class File;
}

class MGFFileTreeModel : public QAbstractItemModel
{
public:
    MGFFileTreeModel(const std::vector<MGF::File>& mgfItems) : m_FileListReference(mgfItems) {}

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    const std::vector<MGF::File>& m_FileListReference;
};

#endif // MGFFILETREEMODEL_H
