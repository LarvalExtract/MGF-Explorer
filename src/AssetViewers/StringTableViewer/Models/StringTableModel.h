#pragma once

#include "MGF/Assets/StringTable.h"

#include <QAbstractTableModel>
#include <vector>

namespace StringTableViewer { namespace Models {

	class StringTableModel : public QAbstractTableModel
	{
	public:
		void SetAssetReference(const MGF::Asset::StringTable* stringTableAsset);

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	private:
		const MGF::Asset::StringTable* StringTableAsset;
	};

} }
