#pragma once

#include "MGF/MGFFile.h"
#include "MGF/Structures/Tif.h"

namespace MGF {	namespace Factories {

	class ImageFactory
	{
	public:
		static void Deserialize(const MGFFile& tif, MA2_TIF_FILE& image, char** bits = nullptr);
		static void Deserialize(const MGFFile& tif, MA1_TIF_FILE& image, char** bits = nullptr);
	};

} }