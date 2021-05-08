#pragma once

#include <QAbstractItemModel>

namespace MGF { namespace Asset { class EntityAsset; } }

namespace EntityViewer { namespace Models {

	class EntityTree : public QAbstractItemModel
	{
		friend class MGF::Asset::EntityAsset;

	public:
		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

		const MGF::Asset::EntityAsset* AssetRef = nullptr;
	};

} }