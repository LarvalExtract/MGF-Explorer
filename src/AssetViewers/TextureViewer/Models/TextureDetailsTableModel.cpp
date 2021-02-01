#include "TextureDetailsTableModel.h"

static const QVariant MA1_TABLE_HEADERS[] = {
	"Width",
	"Height",
	"Flags",
	"Mips",
	"Size"
};

static const QVariant MA2_TABLE_HEADERS[] = {
	"Width",
	"Height",
	"Flags",
	"Mips",
	"Size",
	"Depth",
	"Frames"
}

using namespace TextureViewer::Models;

int TextureDetailsTable::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 1;
}

int TextureDetailsTable::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (TextureAsset.Version() == MGF::Version::MechAssault2LW)
	{
		return sizeof(MA2_TABLE_HEADERS) / sizeof(QVariant);
	}
	else
	{
		return sizeof(MA1_TABLE_HEADERS) / sizeof(QVariant);
	}
}

QVariant TextureDetailsTable::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	switch (index.column())
	{
	case 0: return TextureAsset.Width;
	case 1: return TextureAsset.Height;
	case 2: return QString::number(TextureAsset.Flags, 16);
	case 3: return TextureAsset.Mips;
	case 4: return TextureAsset.Size;
	case 5: return TextureAsset.Depth;
	case 6: return TextureAsset.Frames;
	}
}

QVariant TextureDetailsTable::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}
	if (orientation != Qt::Horizontal)
	{
		return QVariant();
	}

	if (TextureAsset.Version() == MGF::Version::MechAssault2LW)
	{
		return MA2_TABLE_HEADERS[section];
	}
	else
	{
		return MA1_TABLE_HEADERS[section];
	}
}

