#include "MGFMeshTableModel.h"

static const QVariant HEADERS[] = {
    "Name",
    "Material",
    "Vertices",
    "Stride",
    "Flags",
    "Topology"
};

int MGFMeshTableModel::rowCount(const QModelIndex& parent) const
{
    return MeshDefinitions.size();
}

Q_INVOKABLE int MGFMeshTableModel::columnCount(const QModelIndex& parent) const
{
    constexpr int count = sizeof(HEADERS) / sizeof(QVariant);
    return count;
}

Q_INVOKABLE QVariant MGFMeshTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const auto renderOperationTypeString = [](Ogre::RenderOperation::OperationType type)
    {
        using ot = Ogre::RenderOperation::OperationType;
        switch (type)
        {
        case ot::OT_TRIANGLE_LIST: return "Triangle list";
        case ot::OT_TRIANGLE_STRIP: return "Triangle strip";
        default: return "UNKNOWN";
        }
    };

    const auto& meshDef = MeshDefinitions[index.row()];
    switch (index.column())
    {
    case 0: return meshDef.name.c_str();
    case 1: return meshDef.materialPath.c_str();
    case 2: return meshDef.numVerts;
    case 3: return meshDef.stride;
    case 4: return QString::number(meshDef.flags, 16);
    case 5: return renderOperationTypeString(meshDef.topology);
    }
}

QVariant MGFMeshTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    return HEADERS[section];
}
