#pragma once

#include "AssetBase.h"

#include <QString>

namespace MGF {	namespace Asset {

	class PlainText : public AssetBase
	{
	public:
		PlainText(const File& soureFile);
		inline const QString& TextData() const { return Text; }

	private:
		QString Text;
	};

} }