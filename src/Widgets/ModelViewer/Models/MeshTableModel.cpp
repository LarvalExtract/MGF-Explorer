#include "MeshTableModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Vertices",
	"Flags",
	"Stride"
};

constexpr size_t COLUMNS = sizeof(HEADERS) / sizeof(HEADERS[0]);

using namespace ModelViewer::Models;

int MeshTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return size();
}

int MeshTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return COLUMNS;
}

QVariant MeshTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!(index.isValid() && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	const auto& mesh = this->at(index.row());
	switch (index.column())
	{
	case 0: return mesh.name.c_str();
	case 1: return mesh.numVerts;
	case 2: return QString::number(mesh.flags, 16);
	case 3: return mesh.stride;
	}
	
	return QVariant();
}

QVariant MeshTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (!(orientation == Qt::Horizontal && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	return HEADERS[section];
}

