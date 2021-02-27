#include "NodeTreeModel.h"

static const QVariant HEADERS[] = {
	"Name",
	"Type"
};

constexpr auto COLUMN_COUNT = sizeof(HEADERS) / sizeof(HEADERS[0]);

using namespace ModelViewer::Models;

void NodeTree::SetAssetReference(const std::vector<MGF::Asset::Model::Node>* pNodes)
{
	this->pNodes = pNodes;

	emit dataChanged(createIndex(0, 0), createIndex(pNodes->size(), COLUMN_COUNT));
}

QModelIndex NodeTree::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	const MGF::Asset::Model::Node* parentNode = parent.isValid()
		? static_cast<const MGF::Asset::Model::Node*>(parent.internalPointer())
		: &pNodes->at(0);

	if (row < 0 || row >= parentNode->children.size())
	{
		return QModelIndex();
	}

	return createIndex(row, column, parentNode->children[row]);
}

QModelIndex NodeTree::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	const auto childNode = static_cast<const MGF::Asset::Model::Node*>(child.internalPointer());
	if (childNode == &(*pNodes)[0])
	{
		return createIndex(0, 0, (quintptr)childNode);
	}

	const auto parentNode = (quintptr)&(*pNodes)[childNode->parentIndex];
	
	return parentNode ? createIndex(childNode->parentIndex, 0, parentNode) : QModelIndex();
}

int NodeTree::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	const auto* parentNode = parent.isValid()
		? static_cast<const MGF::Asset::Model::Node*>(parent.internalPointer())
		: &pNodes->at(0);

	return parentNode->children.size();
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

	auto& node = pNodes->at(index.row());
	if (role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0: return node.name.c_str();
		case 1: return node.type.c_str();
		}
	}

	else if (role == Qt::CheckStateRole && index.column() == 0)
	{
		return node.bVisible ? Qt::Checked : Qt::Unchecked;
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

int SetVisibility(MGF::Asset::Model::Node& node, bool visible)
{
	int result = 1;

	node.bVisible = visible;
	node.sceneNode->setVisible(visible);

	for (auto& child : node.children)
	{
		result += SetVisibility(*child, visible);
	}

	return result;
}

bool NodeTree::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	if (!(index.isValid() && role == Qt::CheckStateRole))
	{
		return false;
	}

	auto node = *static_cast<MGF::Asset::Model::Node*>(index.internalPointer());
	auto last = SetVisibility(node, value == Qt::Checked);

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

