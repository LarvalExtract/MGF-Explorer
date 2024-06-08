#include "MaterialFactory.h"
#include "MaterialParamReader.h"

#include <QDiffuseSpecularMaterial>
#include <QParameter>
#include <QTechnique>
#include <QGraphicsApiFilter>
#include <QRenderPass>
#include <QUrl>
#include <format>
#include <QDir>

using namespace Qt3DRender;
using namespace Qt3DExtras;

namespace MGF::Render
{
	Qt3DRender::QMaterial* MaterialFactory::CreateMaterial(IMaterialParamReader& paramReader)
	{
		QMaterial* material = new QMaterial;

		QTechnique* technique = new QTechnique;
		technique->setParent(material);
		technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
		technique->graphicsApiFilter()->setMajorVersion(3);
		technique->graphicsApiFilter()->setMinorVersion(3);
		technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

		const auto AddBasePass = [&technique, &paramReader] (const QString& shader) {
			QRenderPass* baseRenderPass = new QRenderPass;
			baseRenderPass->setParent(technique);
			baseRenderPass->addParameter(new QParameter("diffuse_colour", paramReader.ReadColour("diffuse", "diff_color")));
			baseRenderPass->addParameter(new QParameter("selfillum_colour", paramReader.ReadColour("selfillum", "selfillum_color")));
			baseRenderPass->addParameter(new QParameter("shininess", paramReader.ReadFloat("shininess")));
			baseRenderPass->addParameter(new QParameter("specular_colour", paramReader.ReadColour("spec_color")));
			baseRenderPass->addParameter(new QParameter("basetexture", paramReader.ReadTexture("basetexture", "base")));
		
			const auto [blendFunc, blendState] = paramReader.ReadBlendingMode("blending");
			baseRenderPass->addRenderState(blendFunc);
			baseRenderPass->addRenderState(blendState);
			baseRenderPass->addRenderState(paramReader.ReadCullingMode("two_sided"));
		
			QShaderProgram* baseShader = new QShaderProgram;
			baseShader->setParent(baseRenderPass);
			baseShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/" + shader)));
			baseShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Basic.vert")));
			// baseShader->setGeometryShaderCode(); todo - use shading mode property from material to determine normals, in geom shader
			baseRenderPass->setShaderProgram(baseShader);
		
			technique->addRenderPass(baseRenderPass);
		};

		if (const std::string_view materialType = paramReader.ReadMaterialType(); 
			materialType == "base" || materialType == "BASE")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else if (materialType == "specular_mask")
		{
			AddBasePass("SpecularMaskMaterial.frag");
		}
		else if (materialType == "color_shift")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else if (materialType == "complex")
		{
			QRenderPass* baseRenderPass = new QRenderPass;
			baseRenderPass->setParent(technique);
			baseRenderPass->addParameter(new QParameter("diffuse_colour", paramReader.ReadColour("diffuse", "diff_color")));
			baseRenderPass->addParameter(new QParameter("selfillum_colour", paramReader.ReadColour("selfillum", "selfillum_color")));
			baseRenderPass->addParameter(new QParameter("shininess", paramReader.ReadFloat("shininess")));
			baseRenderPass->addParameter(new QParameter("specular_colour", paramReader.ReadColour("spec_color")));
			baseRenderPass->addParameter(new QParameter("basetexture", paramReader.ReadTexture("basetexture", "base")));
			baseRenderPass->addParameter(new QParameter("multitexture", paramReader.ReadTexture("multitexture")));
			baseRenderPass->addParameter(new QParameter("colorshift0", paramReader.ReadColour("colorshift0_color")));
			baseRenderPass->addParameter(new QParameter("colorshift1", paramReader.ReadColour("colorshift1_color")));

			const auto [blendFunc, blendState] = paramReader.ReadBlendingMode("blending");
			baseRenderPass->addRenderState(blendFunc);
			baseRenderPass->addRenderState(blendState);
			baseRenderPass->addRenderState(paramReader.ReadCullingMode("two_sided"));

			QShaderProgram* baseShader = new QShaderProgram;
			baseShader->setParent(baseRenderPass);
			baseShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/ComplexMaterial.frag")));
			baseShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Basic.vert")));
			// baseShader->setGeometryShaderCode(); todo - use shading mode property from material to determine normals, in geom shader
			baseRenderPass->setShaderProgram(baseShader);

			technique->addRenderPass(baseRenderPass);
		}
		else if (materialType == "detail")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else if (materialType == "distort")
		{
			//AddBasePass();
		}
		else if (materialType == "env")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else if (materialType == "solid")
		{
			//AddBasePass();
		}
		else if (materialType == "animated")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else
		{
			qDebug() << "MGF::Render::MaterialFactory - Unrecognised material type" << materialType.data();
		}

		Qt3DRender::QFilterKey* filter = new Qt3DRender::QFilterKey;
		filter->setParent(material);
		filter->setName(QStringLiteral("renderingStyle"));
		filter->setValue(QStringLiteral("forward"));
		technique->addFilterKey(filter);
		
		QEffect* effect = new QEffect;
		effect->setParent(material);
		effect->addTechnique(technique);
		
		material->setEffect(effect);

		return material;
	}

	Qt3DRender::QRenderPass* MaterialFactory::CreateRenderPass(IMaterialParamReader& paramReader)
	{
		return nullptr;
	}
}


