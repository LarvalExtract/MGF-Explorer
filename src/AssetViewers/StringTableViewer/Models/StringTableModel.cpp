#include "StringTableModel.h"

static const QVariant TABLE_HEADERS[] = {
	"ID",
	"Text"
};

using namespace StringTableViewer::Models;

StringTableModel::StringTableModel(const MGF::Asset::StringTable& stringTableAsset) :
	StringTableAsset(stringTableAsset)
{

}

int StringTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return StringTableAsset.Strings.size();
}

int StringTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	constexpr int count = sizeof(TABLE_HEADERS) / sizeof(QVariant);
	return count;
}

QVariant StringTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    return QVariant();
}

QVariant StringTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    return QVariant();
}
