#pragma once

#include "MGF/Assets/ModelAsset.h"

#include <QAbstractItemModel>

namespace ModelViewer {	namespace Models {

	class NodeTree : public QAbstractItemModel
	{
	public:
		void SetAssetReference(const MGF::Asset::ModelAsset* modelAsset);

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	private:
		const MGF::Asset::ModelAsset* ModelAsset = nullptr;
	};

} }