#pragma once

#include "AssetBase.h"

#include <QString>

namespace MGF {	namespace Asset {

	class PlainTextAsset : public AssetBase
	{
	public:
		PlainTextAsset(const File& soureFile);
		QString Text;
	};

} }