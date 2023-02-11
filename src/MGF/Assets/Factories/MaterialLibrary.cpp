#include "MaterialLibrary.h"
#include "MGFExplorerApplication.h"
#include "XmlUtils.h"

#include <QDiffuseSpecularMaterial>
#include <QDiffuseMapMaterial>
#include <QTextureMaterial>
#include <QAbstractTexture>
#include <QTechnique>
#include <QRenderPass>
#include <QRenderState>

#include <QBlendEquation>
#include <QBlendEquationArguments>
#include <QCullFace>

Qt3DRender::QBlendEquationArguments* CreateBlendState(const std::string_view mode)
{
	using namespace Qt3DRender;

	Qt3DRender::QBlendEquationArguments* blend = new Qt3DRender::QBlendEquationArguments;
	if (mode == "normal")
	{
		blend->setSourceRgb(QBlendEquationArguments::One);
		blend->setDestinationRgb(QBlendEquationArguments::Zero);
	}
	else if (mode == "additive")
	{
		blend->setSourceRgb(QBlendEquationArguments::SourceColor);
		blend->setDestinationRgb(QBlendEquationArguments::DestinationColor);
	}
	else if (mode == "alpha")
	{
		blend->setSourceRgb(QBlendEquationArguments::SourceAlpha);
		blend->setDestinationRgb(QBlendEquationArguments::OneMinusSourceAlpha);
	}
	else
	{
		__debugbreak();
	}
	return blend;
}

std::pair<std::string, Qt3DRender::QMaterial*> MA::MaterialLibrary::CreateMaterial(const pugi::xml_node& materialXml, const MGF::File& sourceFile)
{
	const auto material = new Qt3DExtras::QDiffuseSpecularMaterial;

	material->setDiffuse(MA::StrToColor(materialXml.attribute("diffuse").as_string()));
	material->setSpecular(MA::StrToColor(materialXml.attribute("spec_color").as_string()));
	material->setShininess(materialXml.attribute("shininess").as_float());
	material->setAmbient(QColor::fromRgbF(1.0f, 1.0f, 1.0f));
	material->setTextureScale(32.0f);
	//material->setAlphaBlendingEnabled(true);
	const bool twoSided = materialXml.attribute("two_sided").as_bool();

	//auto pass = new Qt3DRender::QRenderPass;
	//pass->addRenderState(CreateBlendState(materialXml.attribute("blending").as_string()));

	//auto cull = new Qt3DRender::QCullFace;
	//cull->setMode(twoSided ? Qt3DRender::QCullFace::Back : Qt3DRender::QCullFace::NoCulling);
	//pass->addRenderState(cull);

	//material->effect()->techniques()[0]->addRenderPass(pass);

	auto GetTexture = [materialXml, sourceFile](const char* texture)
	{
		const auto basetextureNode = materialXml.child(texture);
		const auto textureFile = sourceFile.FindRelativeItem(basetextureNode.attribute("filename").as_string());

		return qApp->mTextureLibrary.GetTexture(*textureFile);
	};

	if (const std::string_view type = materialXml.name(); 
		type.contains("base") || 
		type.contains("complex") || 
		type.contains("detail") || 
		type.contains("specular_mask") ||
		type.contains("color_shift"))
	{
		QVariant var;
		var.setValue(GetTexture("basetexture"));

		material->setDiffuse(var);
	}

	const std::string name = materialXml.attribute("name").as_string();
	return std::make_pair(name, material);
}
