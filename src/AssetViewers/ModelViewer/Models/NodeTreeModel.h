#pragma once

#include "MGF/Assets/Model/Node.h"

#include <QAbstractItemModel>

namespace MGF { namespace Asset { class ModelAsset; } }

namespace ModelViewer {	namespace Models {

	class NodeTree : public QAbstractItemModel
	{
		friend class MGF::Asset::ModelAsset;

	public:
		~NodeTree() { delete RootNode; }

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		Qt::ItemFlags flags(const QModelIndex& index) const override;

	private:
		MGF::Asset::Model::Node* RootNode = nullptr;
	};

} }