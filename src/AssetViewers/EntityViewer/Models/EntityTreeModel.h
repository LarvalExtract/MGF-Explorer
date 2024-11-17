#pragma once

#include <QAbstractItemModel>
#include <QHash>

namespace MGF::Asset { class WdfMap; }

namespace EntityViewer { namespace Models {

	class EntityTreeModel : public QAbstractItemModel
	{
	public:
		EntityTreeModel(const MGF::Asset::WdfMap& InMapAsset);

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	private:
		const MGF::Asset::WdfMap& MapAsset;
	};

} }