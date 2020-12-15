#pragma once

#include <QAbstractTableModel>

#include <vector>

namespace StringTableViewer { namespace Models {

	class StringTableModel : public QAbstractTableModel
	{
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	private:
		std::vector<std::pair<std::uint32_t, std::wstring>> Strings;
	};

} }
