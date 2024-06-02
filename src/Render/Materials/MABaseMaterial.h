#pragma once

#include "../MaterialParamReader.h"

#include <QMaterial>

namespace Qt3DRender
{
	class QRenderPass;
	class QShaderProgram;
}

namespace MGF::Render {

	class MABaseMaterial : public Qt3DRender::QMaterial
	{
	public:
		MABaseMaterial(IMaterialParamReader& params);
		virtual ~MABaseMaterial() override = default;

		Qt3DRender::QParameter* DiffuseColour;
		Qt3DRender::QParameter* SelfIllumColour;
		Qt3DRender::QParameter* Shininess;
		Qt3DRender::QParameter* ShineStrength;
		Qt3DRender::QParameter* SpecularColour;
		Qt3DRender::QParameter* Opacity;
		Qt3DRender::QParameter* BaseTexture;

	protected:
		Qt3DRender::QEffect* mEffect;
		Qt3DRender::QTechnique* mTechnique;
		Qt3DRender::QRenderPass* mRenderPass;
		Qt3DRender::QShaderProgram* mBaseMaterialShader;
	};

}