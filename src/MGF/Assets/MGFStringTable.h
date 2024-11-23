#pragma once

#include "MGFAsset.h"
#include "Widgets/StringTableViewer/Models/StringTableModel.h"

class MGFStringTable : public MGFAsset
{
public:
	MGFStringTable(const MGFFile& file);

	Models::StringTableModel Strings;
};



