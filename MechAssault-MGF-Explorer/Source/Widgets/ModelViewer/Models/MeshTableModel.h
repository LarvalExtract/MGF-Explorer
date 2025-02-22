#pragma once

#include "MGF/Assets/Model/Mesh.h"

#include <QAbstractTableModel>

namespace MGF { namespace Asset { class MGFModel; } }

namespace ModelViewer {	namespace Models {

	using MeshList = std::vector<MGF::Asset::Model::Mesh>;

	class MeshTable : MeshList, public QAbstractTableModel
	{
		friend class MGF::Asset::MGFModel;

	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

} }