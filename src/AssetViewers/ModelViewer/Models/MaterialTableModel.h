#pragma once

#include <QAbstractTableModel>

namespace ModelViewer { namespace Models {

	class MaterialTable : public QAbstractTableModel
	{
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

} }