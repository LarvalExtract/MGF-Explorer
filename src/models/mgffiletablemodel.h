#ifndef MGFFILETABLEMODEL_H
#define MGFFILETABLEMODEL_H

#include <QAbstractTableModel>

namespace MGF {
    class File;
}

class MGFFileTableModel : public QAbstractTableModel
{
public:
    MGFFileTableModel(const std::vector<MGF::File>& mgfItems) : m_FileListReference(mgfItems) {}

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    const std::vector<MGF::File>& m_FileListReference;
};

#endif // MGFFILETABLEMODEL_H
