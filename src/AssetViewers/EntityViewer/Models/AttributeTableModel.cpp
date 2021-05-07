#include "AttributeTableModel.h"

static const QVariant HEADERS[] = {
	"Name", "Value"
};

using namespace EntityViewer::Models;

int AttributeTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return size();
}

int AttributeTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant AttributeTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!(index.isValid() && role == Qt::DisplayRole))
	{
		return QVariant();
	}
}

QVariant AttributeTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		? HEADERS[section]
		: QVariant();
}
