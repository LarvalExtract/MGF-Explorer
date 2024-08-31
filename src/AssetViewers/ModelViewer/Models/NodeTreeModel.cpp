#include "NodeTreeModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type"
};

constexpr auto COLUMN_COUNT = sizeof(HEADERS) / sizeof(HEADERS[0]);

using namespace ModelViewer::Models;

QModelIndex NodeTree::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}
	
	const Qt3DCore::QEntity* parentNode = parent.isValid()
		? static_cast<Qt3DCore::QEntity*>(parent.internalPointer())
		: mRootNode;

	const Qt3DCore::QEntity* childNode = dynamic_cast<Qt3DCore::QEntity*>(parentNode->childNodes()[row]);
	if (childNode)
	{
		return createIndex(row, column, childNode);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex NodeTree::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	const Qt3DCore::QNode* childNode = static_cast<Qt3DCore::QNode*>(child.internalPointer());
	const Qt3DCore::QEntity* childEntity = dynamic_cast<const Qt3DCore::QEntity*>(childNode);
	const Qt3DCore::QEntity* parentNode = static_cast<Qt3DCore::QEntity*>(childEntity->parentEntity());

	if (parentNode == mRootNode || childNode == nullptr)
	{
		return {};
	}

	int n = 0;
	for (const Qt3DCore::QNodeVector children = parentNode->childNodes(); const Qt3DCore::QNode* child : children)
	{
		if (const Qt3DCore::QEntity* entity = dynamic_cast<const Qt3DCore::QEntity*>(child))
		{
			if (entity == childNode)
			{
				break;
			}

			n++;
		}
	}
	
	return createIndex(n, 0, parentNode);
}

int NodeTree::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	const Qt3DCore::QNode* parentNode = parent.isValid()
		? static_cast<Qt3DCore::QNode*>(parent.internalPointer())
		: mRootNode;

	int childCount = 0;
	for (Qt3DCore::QNodeVector children = parentNode->childNodes(); Qt3DCore::QNode * child : children)
	{
		if (dynamic_cast<Qt3DCore::QEntity*>(child) != nullptr)
			childCount++;
	}
	return childCount;
}

int NodeTree::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return COLUMN_COUNT;
}

QVariant NodeTree::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	const const Qt3DCore::QEntity& node = *static_cast<Qt3DCore::QEntity*>(index.internalPointer());
	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0: return node.property("name").toString();
		case 1: return node.property("type").toString();
		}
	}

	else if (role == Qt::CheckStateRole && index.column() == 0)
	{
		return node.isEnabled() ? Qt::Checked : Qt::Unchecked;
	}

	return QVariant();
}

QVariant NodeTree::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (!(role == Qt::DisplayRole && orientation == Qt::Horizontal))
	{
		return QVariant();
	}

	return HEADERS[section];
}

int SetVisibility(Qt3DCore::QEntity* node, bool visible)
{
	int result = 1;

	node->setEnabled(visible);

	for (const Qt3DCore::QNodeVector children = node->childNodes(); Qt3DCore::QNode* child : children)
	{
		if (Qt3DCore::QEntity* childEntity = dynamic_cast<Qt3DCore::QEntity*>(child))
		{
			result += SetVisibility(childEntity, visible);
		}
	}

	return result;
}

bool NodeTree::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	if (!(index.isValid() && role == Qt::CheckStateRole))
	{
		return false;
	}

	Qt3DCore::QEntity* node = static_cast<Qt3DCore::QEntity*>(index.internalPointer());
	int last = SetVisibility(node, value == Qt::Checked);

	emit dataChanged(index, index.siblingAtRow(last));
}

Qt::ItemFlags NodeTree::flags(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return Qt::ItemFlags();
	}

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == 0)
	{
		flags |= Qt::ItemIsUserCheckable;
	}

	return flags;
}

