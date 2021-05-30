#include "TextureDetailsTableModel.h"

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
			case 2: return QString::number(Flags, 16);
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
	return role == Qt::DisplayRole && section == Qt::Horizontal
		? HEADERS[section]
		: QVariant();
}

