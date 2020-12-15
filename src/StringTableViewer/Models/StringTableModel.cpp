#include "StringTableModel.h"

static const QVariant TABLE_HEADERS[] = {
	"ID",
	"Text"
};

using namespace StringTableViewer::Models;

int StringTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return Strings.size();
}

int StringTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	constexpr int count = sizeof(TABLE_HEADERS) / sizeof(QVariant);
	return count;
}

QVariant StringTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{

}

QVariant StringTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	
}
