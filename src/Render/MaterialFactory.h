#pragma once

#include <QMaterial>
#include <QRenderPass>

namespace MGF::Render {

	class IMaterialParamReader;

	class MaterialFactory
	{
	public:
		static Qt3DRender::QMaterial* CreateMaterial(IMaterialParamReader& paramReader);

	private:
		static Qt3DRender::QMaterial* CreateBaseMaterial(IMaterialParamReader& paramReader);
		static Qt3DRender::QRenderPass* CreateRenderPass(IMaterialParamReader& paramReader);

	};

}