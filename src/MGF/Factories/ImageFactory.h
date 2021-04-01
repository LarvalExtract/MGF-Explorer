#pragma once

#include "MGF/File.h"
#include "MGF/Structures/Tif.h"

namespace MGF {	namespace Factories {

	class ImageFactory
	{
	public:
		static void Deserialize(const MGF::File& tif, MA2_TIF_FILE& image);
		static void Deserialize(const MGF::File& tif, MA1_TIF_FILE& image);
	};

} }