#include "EntityTreeModel.h"
#include "MGF/Assets/EntityAsset.h"

static const QVariant HEADERS[] = {
	"Name", "Class", "ID"
};

using namespace EntityViewer::Models;

QModelIndex EntityTree::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	const auto childEntity = (parent.isValid()
		? static_cast<MGF::Asset::Entity*>(parent.internalPointer())
		: AssetRef->RootEntity
	)->Children[row];

	return createIndex(row, column, childEntity);
}

QModelIndex EntityTree::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	auto childEntity = static_cast<MGF::Asset::Entity*>(child.internalPointer());
	auto parentEntity = childEntity->ParentEntity;

	if (!parentEntity)
	{
		return QModelIndex();
	}

	return createIndex(parentEntity->Row(), 0, parentEntity);
}

int EntityTree::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	return (parent.isValid()
		? static_cast<MGF::Asset::Entity*>(parent.internalPointer())
		: AssetRef->RootEntity
	)->Children.size();
}

int EntityTree::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant EntityTree::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return (index.isValid() && role == Qt::DisplayRole) ?
	[&index]() -> QVariant
	{
		switch (const auto entity = static_cast<MGF::Asset::Entity*>(index.internalPointer()); index.column())
		{
		case 0: return entity->Description;
		case 1: return entity->Class;
		case 2: return entity->UID;
		}
	}()	: QVariant();
}

QVariant EntityTree::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? HEADERS[section] : QVariant();
}

