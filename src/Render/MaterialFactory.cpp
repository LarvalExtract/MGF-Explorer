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

		QEffect* effect = new QEffect;
		effect->setParent(material);
		effect->addTechnique(technique);

		QRenderPass* baseRenderPass = new QRenderPass;
		baseRenderPass->setParent(technique);
		const auto [blendFunc, blendState] = paramReader.ReadBlendingMode("blending");
		baseRenderPass->addRenderState(blendFunc);
		baseRenderPass->addRenderState(blendState);
		baseRenderPass->addRenderState(paramReader.ReadCullingMode("two_sided"));
		technique->addRenderPass(baseRenderPass);

		material->setEffect(effect);
		material->setProperty("blending", paramReader.ReadString("blending").data());

		const auto ShadingTypeToInt = [](const std::string_view shadingMode) -> unsigned int {
			if (shadingMode == "gouraud")
			{
				return 0;
			}
			else if (shadingMode == "material_only")
			{
				return 1;
			}
			else if (shadingMode == "translucent")
			{
				return 2;
			}
			else if (shadingMode == "none")
			{
				return 3;
			}
		};

		const auto AddBasePass = [&material, &baseRenderPass, &paramReader, ShadingTypeToInt] (const QString& shader) {
			material->addParameter(new QParameter("diffuse_colour", paramReader.ReadColour("diffuse", "diff_color")));
			material->addParameter(new QParameter("selfillum_colour", paramReader.ReadColour("selfillum", "selfillum_color")));
			material->addParameter(new QParameter("shininess", paramReader.ReadFloat("shininess")));
			material->addParameter(new QParameter("specular_colour", paramReader.ReadColour("spec_color")));
			material->addParameter(new QParameter("basetexture", paramReader.ReadTexture("basetexture", "base")));
			material->addParameter(new QParameter("shadingTypeEnum", ShadingTypeToInt(paramReader.ReadString("shading"))));
		
			QShaderProgram* baseShader = new QShaderProgram;
			baseShader->setParent(baseRenderPass);
			baseShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/" + shader)));
			baseShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Basic.vert")));
			baseRenderPass->setShaderProgram(baseShader);
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
			material->addParameter(new QParameter("diffuse_colour", paramReader.ReadColour("diffuse", "diff_color")));
			material->addParameter(new QParameter("selfillum_colour", paramReader.ReadColour("selfillum", "selfillum_color")));
			material->addParameter(new QParameter("shininess", paramReader.ReadFloat("shininess")));
			material->addParameter(new QParameter("specular_colour", paramReader.ReadColour("spec_color")));
			material->addParameter(new QParameter("basetexture", paramReader.ReadTexture("basetexture", "base")));
			material->addParameter(new QParameter("multitexture", paramReader.ReadTexture("multitexture")));
			material->addParameter(new QParameter("colorshift0", paramReader.ReadColour("colorshift0_color")));
			material->addParameter(new QParameter("colorshift1", paramReader.ReadColour("colorshift1_color")));
			material->addParameter(new QParameter("shadingTypeEnum", ShadingTypeToInt(paramReader.ReadString("shading"))));

			QShaderProgram* baseShader = new QShaderProgram;
			baseShader->setParent(baseRenderPass);
			baseShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/ComplexMaterial.frag")));
			baseShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Basic.vert")));
			baseRenderPass->setShaderProgram(baseShader);
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
			material->addParameter(new QParameter("diffuse_colour", paramReader.ReadColour("diffuse", "diff_color")));
			material->addParameter(new QParameter("selfillum_colour", paramReader.ReadColour("selfillum", "selfillum_color")));
			material->addParameter(new QParameter("shininess", paramReader.ReadFloat("shininess")));
			material->addParameter(new QParameter("specular_colour", paramReader.ReadColour("spec_color")));
			material->addParameter(new QParameter("shadingTypeEnum", ShadingTypeToInt(paramReader.ReadString("shading"))));

			QShaderProgram* baseShader = new QShaderProgram;
			baseShader->setParent(baseRenderPass);
			baseShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/SolidMaterial.frag")));
			baseShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Basic.vert")));
			baseRenderPass->setShaderProgram(baseShader);
		}
		else if (materialType == "animated")
		{
			AddBasePass("BaseMaterial.frag");
		}
		else
		{
			qDebug() << "MGF::Render::MaterialFactory - Unrecognised material type" << materialType.data();
		}
		

		return material;
	}

	Qt3DRender::QRenderPass* MaterialFactory::CreateRenderPass(IMaterialParamReader& paramReader)
	{
		return nullptr;
	}
}


