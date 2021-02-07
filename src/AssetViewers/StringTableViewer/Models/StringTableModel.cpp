#include "StringTableModel.h"

static const QVariant TABLE_HEADERS[] = {
	"ID",
	"Text"
};

using namespace StringTableViewer::Models;

void StringTableModel::SetAssetReference(const MGF::Asset::StringTable* stringTableAsset)
{
	StringTableAsset = stringTableAsset;

	emit dataChanged(createIndex(0, 0), createIndex(StringTableAsset->GetStrings().size(), sizeof(TABLE_HEADERS)));
}

int StringTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return StringTableAsset->GetStrings().size();
}

int StringTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	constexpr int count = sizeof(TABLE_HEADERS) / sizeof(QVariant);
	return count;
}

QVariant StringTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	switch (index.column())
	{
	case 0: return StringTableAsset->GetStrings()[index.row()].key;
	case 1: return StringTableAsset->GetStrings()[index.row()].str;
	}
}

QVariant StringTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	if (orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	return TABLE_HEADERS[section];
}
