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
		DiffuseColour->setValue(params.ReadVector3("diffuse"));
		SelfIllumColour->setValue(params.ReadVector3("selfillum"));
		Shininess->setValue(params.ReadFloat("shininess"));
		ShineStrength->setValue(params.ReadFloat("shin_strength"));
		SpecularColour->setValue(params.ReadVector3("spec_color"));
		BaseTexture->setValue(QVariant::fromValue(params.ReadTexture("basetexture")));

		mBaseMaterialShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl("Shaders/BaseMaterial.frag")));
		mRenderPass->setShaderProgram(mBaseMaterialShader);

		QCullFace* cullMode = new QCullFace();
		cullMode->setMode(params.ReadBool("two_sided") ? QCullFace::NoCulling : QCullFace::Back);
		mRenderPass->addRenderState(cullMode);

		const BlendingType blendType = IMaterialParamReader::BlendingTypeFromString(params.ReadString("blending"));
		QBlendEquation* blendEquation = new QBlendEquation();
		QBlendEquationArguments* blendState = new QBlendEquationArguments();
		if (blendType == BlendingType::Additive)
		{
			blendEquation->setBlendFunction(QBlendEquation::Add);
			blendState->setSourceRgb(QBlendEquationArguments::One);
			blendState->setDestinationRgb(QBlendEquationArguments::One);
		}
		else if (blendType == BlendingType::Normal)
		{
			blendEquation->setBlendFunction(QBlendEquation::Add);
			blendState->setSourceRgb(QBlendEquationArguments::SourceAlpha);
			blendState->setDestinationRgb(QBlendEquationArguments::OneMinusSourceAlpha);
		}
		else if (blendType == BlendingType::Alpha)
		{

		}
		
		mRenderPass->addRenderState(blendEquation);
		mRenderPass->addRenderState(blendState);

		mTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
		mTechnique->graphicsApiFilter()->setMajorVersion(3);
		mTechnique->graphicsApiFilter()->setMinorVersion(3);
		mTechnique->addRenderPass(mRenderPass);
		mEffect->addTechnique(mTechnique);

		mEffect->addParameter(DiffuseColour);
		mEffect->addParameter(SelfIllumColour);
		mEffect->addParameter(Shininess);
		mEffect->addParameter(ShineStrength);
		mEffect->addParameter(SpecularColour);
		mEffect->addParameter(BaseTexture);

		setEffect(mEffect);
	}
}