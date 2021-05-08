#pragma once

#include "FileExtractItem.h"

#include <QAbstractTableModel>

namespace FileExtractor { namespace Models {

	class FileExtractListModel : public QAbstractTableModel
	{
	public:
		FileExtractListModel(std::vector<FileExtractItem>& fileExtractQueue) :
			FileExtractQueue(fileExtractQueue) {}

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

		std::vector<FileExtractItem>& FileExtractQueue;
	};

} }