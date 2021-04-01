#pragma once

#include "MGF/Assets/Model/Animation.h"

#include <QAbstractTableModel>

namespace ModelViewer {	namespace Models {

	using AnimationList = std::vector<MGF::Asset::Model::Animation>;

	class AnimationTableModel : public AnimationList, public QAbstractTableModel
	{
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

} }