#include "MGFMaterialTableModel.h"

static const QVariant HEADERS[] = {
    "Name",
    "Type",
    "Diffuse",
    "Specular",
    "Ambient"
};

int MGFMaterialTableModel::rowCount(const QModelIndex& parent) const
{
    return MaterialDefinitions.size();
}

int MGFMaterialTableModel::columnCount(const QModelIndex& parent) const
{
    constexpr int count = sizeof(HEADERS) / sizeof(QVariant);
    return count;
}

QVariant MGFMaterialTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const auto rgbaToString = [](std::uint32_t rgba)
    {
        constexpr std::uint32_t r_mask = 0xFF000000;
        constexpr std::uint32_t g_mask = r_mask >> 8;
        constexpr std::uint32_t b_mask = g_mask >> 8;
        return (
            QString::number((rgba & r_mask) >> 24) + ' ' + 
            QString::number((rgba & g_mask) >> 16) + ' ' + 
            QString::number((rgba & b_mask) >> 8)
            );
    };

    const auto& m = MaterialDefinitions[index.row()];
    switch (index.column())
    {
    case 0: return m.name.c_str();
    case 1: return m.type.c_str();
    case 2: return rgbaToString(m.diffuse.getAsRGBA());
    case 3: return rgbaToString(m.specular.getAsRGBA());
    case 4: return rgbaToString(m.ambient.getAsRGBA());
    }
}

QVariant MGFMaterialTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    return HEADERS[section];
}
