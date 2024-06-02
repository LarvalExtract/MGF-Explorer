#include "MaterialFactory.h"
#include "MaterialParamReader.h"

#include <QDiffuseSpecularMaterial>
#include "Materials/MABaseMaterial.h"

namespace MGF::Render
{
	Qt3DRender::QMaterial* MaterialFactory::CreateMaterial(IMaterialParamReader& paramReader)
	{
		if (const std::string_view materialType = paramReader.ReadString("type"); 
			materialType == "base" || materialType == "BASE")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "color_shift")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "complex")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "detail")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "distort")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "env")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "solid")
		{
			return new MABaseMaterial(paramReader);
		}
		else if (materialType == "animated")
		{
			return new MABaseMaterial(paramReader);
		}
		else
		{
			throw std::runtime_error("MGF::Render::MaterialFactory - Unrecognised material type '%s'");
		}
	}
}


