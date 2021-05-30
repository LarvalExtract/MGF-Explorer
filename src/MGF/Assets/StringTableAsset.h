#pragma once

#include "AssetBase.h"
#include "AssetViewers/StringTableViewer/Models/StringTableModel.h"

namespace MGF {	namespace Asset {

	class StringTableAsset : public AssetBase
	{
	public:
		StringTableAsset(const File& file);

		StringTableViewer::Models::StringTableModel Strings;
	};

} }


