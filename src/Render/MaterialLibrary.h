#pragma once

#include <QMap>
#include <QStringView>

namespace Qt3DRender {
	class QMaterial;
}

namespace MGF {
	class File;
}

namespace pugi {
	class xml_node;
}

namespace MGF::Render {

	class IMaterialParamReader;

	class MaterialLibrary final
	{
	public:
		static MaterialLibrary& Get();

		Qt3DRender::QMaterial* GetMaterial(const pugi::xml_node& materialNode, const MGF::File& parentFile);
		Qt3DRender::QMaterial* GetMaterial(const MGF::File& materialFile, const MGF::File& parentFile);

	private:
		MaterialLibrary() = default;

		Qt3DRender::QMaterial* GetMaterial(IMaterialParamReader& paramReader);

		QMap<QString, Qt3DRender::QMaterial*> mMaterialCache;
	};

}