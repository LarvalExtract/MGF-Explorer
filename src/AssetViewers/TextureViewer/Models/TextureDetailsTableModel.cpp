#include "TextureDetailsTableModel.h"

#include <QAbstractTexture>

static const QVariant HEADERS[] = {
	"Width",
	"Height",
	"Flags",
	"Mips",
	"Size",
	"Depth",
	"Frames"
};

using namespace TextureViewer::Models;

QVariant ToString(Qt3DRender::QAbstractTexture::TextureFormat format)
{
	using namespace Qt3DRender;

	switch (format)
	{
	case QAbstractTexture::TextureFormat::RGBA8_UNorm:
		return "RGBA8";
	case QAbstractTexture::TextureFormat::R5G6B5:
		return "RGB565";
	case QAbstractTexture::TextureFormat::RGBA4:
		return "RGBA4";
	case QAbstractTexture::TextureFormat::R8_UNorm:
		return "Lum8";
	case QAbstractTexture::TextureFormat::R16_UNorm:
		return "Lum16";
	case QAbstractTexture::TextureFormat::RGBA_DXT1:
		return "DXT1";
	case QAbstractTexture::TextureFormat::RGBA_DXT3:
		return "DXT3";
	case QAbstractTexture::TextureFormat::RGBA_DXT5:
		return "DXT5";
	}
}

int TextureDetailsTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 1;
}

int TextureDetailsTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
}

QVariant TextureDetailsTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return index.isValid() && role == Qt::DisplayRole
		? [this, index]() -> QVariant {
			switch (index.column())
			{
			case 0: return Width;
			case 1: return Height;
			case 2: return ToString(Format);
			case 3: return Mips;
			case 4: return Size;
			case 5: return Depth;
			case 6: return Frames;
			}
		}()
		: QVariant();
}

QVariant TextureDetailsTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return role == Qt::DisplayRole && orientation == Qt::Horizontal
		? HEADERS[section]
		: QVariant();
}

