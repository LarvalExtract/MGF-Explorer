#include "AnimationTableModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Animation"
};

using namespace ModelViewer::Models;

int ModelViewer::Models::AnimationTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return size();
}

int ModelViewer::Models::AnimationTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant ModelViewer::Models::AnimationTableModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!(index.isValid() && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	const auto& anim = this->at(index.row());
	switch (index.column())
	{
	case 0: return anim.name.c_str();
	case 1: return anim.animation.c_str();
	}

	return QVariant();
}

QVariant ModelViewer::Models::AnimationTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (!(orientation == Qt::Horizontal && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	return HEADERS[section];
}

