#pragma once

#include "mgf/AssetBase.h"

#include <string>

namespace MGF {	namespace Asset {

	class PlainText : public AssetBase
	{
	public:
		PlainText(const File&);

	private:
		std::string Text;
	};

} }