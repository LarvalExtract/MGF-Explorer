#include "MaterialLibrary.h"
#include "MaterialFactory.h"
#include "MaterialParamReader.h"

namespace MGF::Render
{
	MaterialLibrary& MaterialLibrary::Get()
	{
		static MaterialLibrary instance;
		return instance;
	}

	Qt3DRender::QMaterial* MGF::Render::MaterialLibrary::GetMaterial(const pugi::xml_node& materialNode, const MGFFile& parentFile)
	{
		MGFMaterialNodeXMLReader paramReader(materialNode, parentFile);
		return GetMaterial(paramReader);
	}

	Qt3DRender::QMaterial* MGF::Render::MaterialLibrary::GetMaterial(const MGFFile& materialFile, const MGFFile& parentFile)
	{
		MGFMaterialFileReader paramReader(materialFile, parentFile);
		return GetMaterial(paramReader);
	}

	Qt3DRender::QMaterial* MaterialLibrary::GetMaterial(IMaterialParamReader& paramReader)
	{
		return MaterialFactory::CreateMaterial(paramReader);
		//const QString materialName = paramReader.ReadString("name").data();
		//if (!mMaterialCache.contains(materialName))
		//{
		//	mMaterialCache.insert(materialName, );
		//}
		//
		//return mMaterialCache[materialName];
	}

}