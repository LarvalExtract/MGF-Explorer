#pragma once

#include "MGF/Assets/ModelAsset.h"

#include <QAbstractItemModel>

namespace ModelViewer {	namespace Models {

	class NodeTree : public QAbstractItemModel
	{
	public:
		void SetAssetReference(const std::vector<MGF::Asset::Model::Node>* pNodes);

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		Qt::ItemFlags flags(const QModelIndex& index) const override;

	private:
		const std::vector<MGF::Asset::Model::Node>* pNodes = nullptr;
	};

} }