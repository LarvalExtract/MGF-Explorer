#pragma once

#include "MGF/Assets/Texture.h"

#include <QAbstractTableModel>

namespace TextureViewer { namespace Models {

	class TextureDetailsTable : public QAbstractTableModel
	{
	public:
		TextureDetailsTable(const MGF::Asset::Texture& textureAsset) : TextureAsset(textureAsset) {}

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	private:
		const MGF::Asset::Texture& TextureAsset;
	};

} }