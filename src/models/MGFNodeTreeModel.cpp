#include "MGFNodeTreeModel.h"

#include <OgreSceneNode.h>

static const QVariant HEADERS[] = {
    "Name",
    "Type"
};

QModelIndex MGFNodeTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column, RootNode->getChild(row));

    auto parentNode = static_cast<Ogre::SceneNode*>(parent.internalPointer());
    auto childNode = parentNode->getChild(row);

    return createIndex(row, column, childNode);
}

QModelIndex MGFNodeTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(child.internalPointer());
    Ogre::SceneNode* parentNode = childNode->getParentSceneNode();

    if (!parentNode)
        return QModelIndex();

    else if (parentNode == RootNode)
        return createIndex(0, 0, RootNode);

    auto GetIndexOf = [](Ogre::SceneNode* node) -> int
    {
        auto parent = node->getParentSceneNode();

        if (!parent)
            return 0;

        int result = 0;
        for (const auto n : parent->getChildren())
        {
            if (n == node)
                break;

            result++;
        }

        return result;
    };

    return createIndex(GetIndexOf(parentNode), 0, parentNode);
}

int MGFNodeTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    Ogre::SceneNode* parentNode = (parent.isValid()) ? static_cast<Ogre::SceneNode*>(parent.internalPointer()) : RootNode;

    if (!parentNode)
        return 0;

    return parentNode->getChildren().size();
}

int MGFNodeTreeModel::columnCount(const QModelIndex& parent) const
{
    constexpr int count = sizeof(HEADERS) / sizeof(QVariant);
    return count;
}

QVariant MGFNodeTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(index.internalPointer());

    auto test = [&node]()
    {
        const auto& str = node->getName();

        auto u1 = str.find('@');
        auto u2 = str.find('@', u1 + 1);

        QString name(str.substr(0, u1).c_str());
        QString nodeType(str.substr(u1 + 1, u2 - u1 - 1).c_str());

        std::pair<QString, QString> result = std::make_pair(name, nodeType);
        return result;
    };

    if (role == Qt::DisplayRole)
    {
        auto values = test();

        switch (index.column())
        {
        case 0: return values.first;
        case 1: return values.second;
        }
    }

    else if (role == Qt::CheckStateRole && index.column() == 0)
    {
        auto sceneNode = static_cast<Ogre::SceneNode*>(index.internalPointer());

        const auto& val = sceneNode->getUserObjectBindings().getUserAny("isVisible");
        bool isVisible = Ogre::any_cast<bool>(val);

        if (isVisible)
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }

    return QVariant();
}

QVariant MGFNodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case 0: return "Name";
    case 1: return "Type";
    }

    return QVariant();
}

Qt::ItemFlags MGFNodeTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemFlags();

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

int SetVisibility(Ogre::SceneNode*, bool);

bool MGFNodeTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::CheckStateRole)
        return false;

    auto sceneNode = static_cast<Ogre::SceneNode*>(index.internalPointer());

    int last = 0;
    if (value == Qt::Checked)
        last = SetVisibility(sceneNode, true);
    else if (value == Qt::Unchecked)
        last = SetVisibility(sceneNode, false);

    emit dataChanged(index, index.sibling(last, 0));

    return true;
}

int SetVisibility(Ogre::SceneNode* node, bool visible)
{
    int result = 1;

    node->setVisible(visible, false);
    node->getUserObjectBindings().setUserAny("isVisible", Ogre::Any(visible));

    for (auto child : node->getChildren())
    {
        result += SetVisibility(static_cast<Ogre::SceneNode*>(child), visible);
    }

    return result;
}