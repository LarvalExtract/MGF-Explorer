#pragma once

#include <QAbstractTableModel>
#include <vector>

namespace Models {

	using StringTable = std::vector<std::pair<uint32_t, QString>>;

	class StringTableModel : public StringTable, public QAbstractTableModel
	{
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

} 
