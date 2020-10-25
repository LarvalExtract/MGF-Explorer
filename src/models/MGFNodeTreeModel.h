#ifndef MGFNODETREEMODEL_H
#define MGFNODETREEMODEL_H

#include <QAbstractItemModel>

namespace Ogre { class SceneNode; };

// Displays node information in node tree widget
class MGFNodeTreeModel : public QAbstractItemModel
{
public: 
    MGFNodeTreeModel() = default;
    MGFNodeTreeModel(const MGFNodeTreeModel&) = default;
    MGFNodeTreeModel(MGFNodeTreeModel&&) = default;
    MGFNodeTreeModel& operator=(const MGFNodeTreeModel&) = default;
    MGFNodeTreeModel& operator=(MGFNodeTreeModel&&) = default;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    Ogre::SceneNode* RootNode = nullptr;
};

#endif // !MGFNODETREEMODEL_H