#include "NodeTreeModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type"
};

using namespace ModelViewer::Models;

void NodeTree::SetAssetReference(const MGF::Asset::ModelAsset* modelAsset)
{
	ModelAsset = modelAsset;
}

QModelIndex NodeTree::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	return QModelIndex();
}

QModelIndex NodeTree::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int NodeTree::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 0;
}

int NodeTree::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant NodeTree::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

QVariant NodeTree::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	if (orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	return HEADERS[section];
}


