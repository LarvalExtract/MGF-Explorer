#ifndef MGFFILETREEMODEL_H
#define MGFFILETREEMODEL_H

#include <QAbstractItemModel>

namespace MGF {
    class File;
}

namespace ArchiveViewer { namespace Models {

	class ArchiveFileTreeModel : public QAbstractItemModel
	{
	public:
		ArchiveFileTreeModel(const std::vector<MGF::File>& mgfItems) : FileListReference(mgfItems) {}

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		QModelIndex index(int row, int column, const QModelIndex& parent) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent) const override;
		int columnCount(const QModelIndex& parent) const override;
		QVariant data(const QModelIndex& index, int role) const override;

	private:
		const std::vector<MGF::File>& FileListReference;
	};

} }


#endif // MGFFILETREEMODEL_H
