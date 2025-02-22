#pragma once

#include "FileExtractItem.h"

#include <QAbstractTableModel>

namespace FileExtractor { namespace Models {

	using FileItemList = std::vector<FileExtractItem>;

	class FileExtractListModel : public FileItemList, public QAbstractTableModel
	{
	public:
		FileExtractListModel(const FileItemList& list) : FileItemList{list} {}

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

} }