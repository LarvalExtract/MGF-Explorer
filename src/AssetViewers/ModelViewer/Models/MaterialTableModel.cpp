#include "MaterialTableModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type"
};

constexpr auto COLUMN_COUNT = sizeof(HEADERS) / sizeof(HEADERS[0]);

using namespace ModelViewer::Models;

void MaterialTable::SetAssetReference(const std::vector<MGF::Asset::Model::Material>* pMaterials)
{
	this->pMaterials = pMaterials;

	emit dataChanged(createIndex(0, 0), createIndex(pMaterials->size(), sizeof(HEADERS)));
}

int MaterialTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return pMaterials->size();
}

int MaterialTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return COLUMN_COUNT;
}

QVariant MaterialTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!(index.isValid() && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	const auto& material = pMaterials->at(index.row());
	switch (index.column())
	{
	case 0: return material.name.c_str();
	case 1: return material.type.c_str();
	}

	return QVariant();
}

QVariant MaterialTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (!(orientation == Qt::Horizontal && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	return HEADERS[section];
}

