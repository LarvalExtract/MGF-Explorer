#include "StringTableFactory.h"
#include "mgf/mgftreeitem.h"

#include "StringTableViewer/Models/StringTableModel.h"

enum HeaderOffsets
{
	TEXT_DATA_SIZE = 64,
	TEXT_STRING_COUNT = 68,
	TEXT_FIRST_STRING_ID = 72,
	TEXT_LAST_STRING_ID = 76,
	TEXT_STRING_DATA = 84
};

using namespace StringTableViewer::Factories;

QAbstractItemModelPtr StringTableFactory::Create(const MGFTreeItem& file)
{


	std::shared_ptr<Models::StringTableModel> StringTable = std::make_shared<Models::StringTableModel>();
	
	return StringTable;
}
