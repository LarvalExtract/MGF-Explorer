#include "StringTableModel.h"

static const QVariant HEADERS[] = {
	"ID",
	"Text"
};

using namespace StringTableViewer::Models;

int StringTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return size();
}

int StringTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant StringTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return index.isValid() && role == Qt::DisplayRole
		? [this, index]() -> QVariant {
			switch (const auto& [key, string] = this->at(index.row()); index.column())
			{
			case 0: return key;
			case 1: return string;
			}
		}()
		: QVariant();
}

QVariant StringTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return role == Qt::DisplayRole && orientation == Qt::Horizontal
		? HEADERS[section]
		: QVariant();
}
