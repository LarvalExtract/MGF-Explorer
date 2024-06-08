#include "MABaseMaterial.h"

#include <QEffect>
#include <QTechnique>
#include <QRenderPass>
#include <QParameter>
#include <QVector3D>
#include <QAbstractTexture>
#include <QCullFace>
#include <QBlendEquation>
#include <QBlendEquationArguments>
#include <QGraphicsApiFilter>
#include <QShaderProgram>

#include <QUrl>

using namespace Qt3DRender;

namespace MGF::Render {

	MABaseMaterial::MABaseMaterial(IMaterialParamReader& params)
		: DiffuseColour(new QParameter(QStringLiteral("diffuse_colour"), QColor::fromRgb(255, 255, 255)))
		, SelfIllumColour(new QParameter(QStringLiteral("selfillum_colour"), QColor::fromRgb(255, 255, 255)))
		, Shininess(new QParameter(QStringLiteral("shininess"), 1.0f))
		, SpecularColour(new QParameter(QStringLiteral("spec_colour"), QVariant()))
		, BaseTexture(new QParameter(QStringLiteral("basetexture"), QVariant()))
		, mEffect(new QEffect())
		, mTechnique(new QTechnique())
		, mRenderPass(new QRenderPass())
		, mBaseMaterialShader(new QShaderProgram())
	{
		DiffuseColour->setValue(params.ReadVector3("diffuse", "diff_color"));
		SelfIllumColour->setValue(params.ReadVector3("selfillum", "selfillum_color"));
		Shininess->setValue(params.ReadFloat("shininess"));
		ShineStrength->setValue(params.ReadFloat("shin_strength"));
		SpecularColour->setValue(params.ReadVector3("spec_color"));
		BaseTexture->setValue(QVariant::fromValue(params.ReadTexture("basetexture", "base")));

		
		mEffect->addTechnique(mTechnique);

		mEffect->addParameter(DiffuseColour);


		setEffect(mEffect);
	}
}