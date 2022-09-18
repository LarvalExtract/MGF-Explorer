#include "FileExtractListModel.h"

#include <QLocale>

static const QVariant HEADERS[] = {
	"File path",
	"Size",
	"Status"
};

static const QVariant STATUS[] = {
	"Queued",
	"Done",
	"Skipped",
	"Failed"
};

using namespace FileExtractor::Models;

int FileExtractListModel::rowCount(const QModelIndex& parent) const
{
	return size();
}

int FileExtractListModel::columnCount(const QModelIndex& parent) const
{
	constexpr int count = sizeof(HEADERS) / sizeof(HEADERS[0]);
	return count;
}

QVariant FileExtractListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	const auto& file = this->at(index.row());
	switch (index.column())
	{
	case 0: return file.mgfItem->FilePath().u8string().c_str();
	case 1: return QLocale::system().formattedDataSize(file.mgfItem->FileLength);
	case 2: return STATUS[(int)file.status];
	}
}

QVariant FileExtractListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	return HEADERS[section];
}
