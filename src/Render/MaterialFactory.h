#pragma once

#include <QMaterial>

namespace MGF::Render {

	class IMaterialParamReader;

	class MaterialFactory
	{
	public:
		static Qt3DRender::QMaterial* CreateMaterial(IMaterialParamReader& paramReader);
	};

}