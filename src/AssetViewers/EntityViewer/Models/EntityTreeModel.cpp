#include "EntityTreeModel.h"
#include "MGF/Assets/MapAsset.h"

static const QVariant HEADERS[] = {
	"Name", "Class", "ID"
};

using namespace EntityViewer::Models;
using namespace MGF::Asset;

EntityTreeModel::EntityTreeModel(const MGF::Asset::WdfMap& InMapAsset) :
	MapAsset(InMapAsset)
{
}

QModelIndex EntityTreeModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	const WdfEntity& parentEntity = parent.isValid() ? *static_cast<WdfEntity*>(parent.internalPointer()) : MapAsset.Objects.at(MapAsset.RootEntityUid);

	return createIndex(row, column, parentEntity.Children[row]);
}

QModelIndex EntityTreeModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	const WdfEntity* pThisEntity = static_cast<WdfEntity*>(child.internalPointer());
	if (MapAsset.Objects.contains(pThisEntity->ParentUID))
	{
		const WdfEntity& ParentEntityRef = MapAsset.Objects.at(pThisEntity->ParentUID);
		const WdfEntity* ParentEntityPtr = &ParentEntityRef;

		const auto it = std::find(ParentEntityRef.Children.begin(), ParentEntityRef.Children.end(), pThisEntity);

		return createIndex(it - ParentEntityRef.Children.begin(), 0, ParentEntityPtr);
	}

	return QModelIndex();
}

int EntityTreeModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	if (parent.isValid())
	{
		const WdfEntity* entity = static_cast<WdfEntity*>(parent.internalPointer());
		return entity->Children.size();
	}
	else
	{
		const WdfEntity& entity = MapAsset.Objects.at(MapAsset.RootEntityUid);
		return entity.Children.size();
	}
}

int EntityTreeModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant EntityTreeModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (index.isValid() && role == Qt::DisplayRole)
	{
		if (const WdfEntity* entity = static_cast<WdfEntity*>(index.internalPointer()))
		{
			switch (index.column())
			{
			case 0: return entity->Description.c_str();
			case 1: return entity->Class.c_str();
			case 2: return entity->UID;
			}
		}
	}
		
	return QVariant();
}

QVariant EntityTreeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? HEADERS[section] : QVariant();
}
