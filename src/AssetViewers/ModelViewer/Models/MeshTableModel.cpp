#include "MeshTableModel.h"

using namespace ModelViewer::Models;

void MeshTable::SetAssetReference(const MGF::Asset::ModelAsset* modelAsset)
{
	ModelAsset = modelAsset;
}

int MeshTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 0;
}

int MeshTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 0;
}

QVariant MeshTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

QVariant MeshTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

