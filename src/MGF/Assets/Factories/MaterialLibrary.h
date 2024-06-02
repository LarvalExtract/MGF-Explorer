#pragma once

#include "MGF/File.h"

#include <unordered_map>
#include <pugixml-1.10/src/pugixml.hpp>

namespace Qt3DRender
{
	class QMaterial;
}

namespace MA
{
	class MaterialLibrary final
	{
	public:
		std::pair<std::string, Qt3DRender::QMaterial*> CreateMaterial(const pugi::xml_node& materialXml, const MGF::File& sourceFile);
		Qt3DRender::QMaterial* CreateMaterial(const MGF::File& matFile, const MGF::File& sourceFile);
	
	};
}