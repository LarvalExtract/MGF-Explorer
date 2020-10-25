#include "MGFAnimationTableModel.h"

static const QVariant HEADERS[] = {
    "Name",
    "Path"
};

int MGFAnimationTableModel::rowCount(const QModelIndex& parent) const
{
    return AnimationDefinitions.size();
}

int MGFAnimationTableModel::columnCount(const QModelIndex& parent) const
{
    constexpr int count = sizeof(HEADERS) / sizeof(HEADERS[0]);
    return count;
}

QVariant MGFAnimationTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const auto& animDef = AnimationDefinitions[index.row()];
    switch (index.column())
    {
    case 0: return animDef.name.c_str();
    case 1: return animDef.animation.c_str();
    }
}

QVariant MGFAnimationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    return HEADERS[section];
}