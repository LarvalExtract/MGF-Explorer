#pragma once

#include "MGF/VersionConstants.h"

#include <QAbstractTableModel>
#include <QAbstractTexture>

namespace TextureViewer { namespace Models {

	class TextureDetailsTable : public QAbstractTableModel
	{
	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

		MGF::Version Version;
		std::uint32_t Width;
		std::uint32_t Height;
		std::uint32_t Depth;
		std::uint32_t Frames;
		std::uint32_t Mips;
		std::uint32_t Size;
		Qt3DRender::QAbstractTexture::TextureFormat Format;
	};

} }