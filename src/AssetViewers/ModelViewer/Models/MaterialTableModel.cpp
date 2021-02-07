#include "MaterialTableModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type"
};

using namespace ModelViewer::Models;

void MaterialTable::SetAssetReference(const MGF::Asset::ModelAsset* modelAsset)
{
	ModelAsset = modelAsset;
}

int MaterialTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 0;
}

int MaterialTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 0;
}

QVariant MaterialTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

QVariant MaterialTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

