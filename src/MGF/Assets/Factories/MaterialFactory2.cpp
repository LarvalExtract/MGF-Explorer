#include "MaterialFactory2.h"

#include <filesystem>
#include <fstream>

#include <QMaterial>
#include <QParameter>
#include <QEffect>
#include <QRenderPass>
#include <QTechnique>
#include <QShaderProgram>

using namespace MA;
using namespace Qt3DRender;

static const std::filesystem::path gShaderFolder{ "shaders" };

MaterialFactory::MaterialFactory()
{
	// Load and compile shaders
	mShaderBase = CreateShaderProgram("", "material_base.glsl");
	// mShaderSolid = CreateShaderProgram("", "material_base.glsl");
	// mShaderBase = CreateShaderProgram("", "material_base.glsl");
	// mShaderComplex = CreateShaderProgram("", "material_base.glsl");
	// mShaderColorShift = CreateShaderProgram("", "material_base.glsl");
	// mShaderSpecularMask = CreateShaderProgram("", "material_base.glsl");
	// mShaderSpecularAlpha = CreateShaderProgram("", "material_base.glsl");
	// mShaderEnv = CreateShaderProgram("", "material_base.glsl");
	// mShaderDetail = CreateShaderProgram("", "material_base.glsl");
	// mShaderDistort = CreateShaderProgram("", "material_base.glsl");
	// mShaderAnimated = CreateShaderProgram("", "material_base.glsl");
	// mShaderDiffuseShift = CreateShaderProgram("", "material_base.glsl");
	
}

MaterialFactory::~MaterialFactory()
{
}

Material MaterialFactory::CreateMaterial(const pugi::xml_node& materialXmlNode, const MGF::File& sourceFile)
{
	if (const std::string_view name = materialXmlNode.name(); name.contains("base"))
	{
		return { MaterialType::Base, CreateMaterialBase(materialXmlNode) };
	}
}

Material MaterialFactory::CreateMaterial(const MGF::File& matFile, const MGF::File& sourceFile)
{
	return {};
}

QMaterial* MaterialFactory::CreateMaterialBase(const pugi::xml_node& materialXml)
{
	const auto ColourFromXmlAttribute = [](const pugi::xml_attribute& attrib) -> QColor
	{
		const std::string_view str = attrib.as_string();

		auto c1 = str.find(',');
		auto c2 = str.find(',', c1 + 1);

		return QColor::fromRgbF(
			std::stof(str.substr(0, c1).data()) / 255.0f,
			std::stof(str.substr(c1 + 1, c2).data()) / 255.0f,
			std::stof(str.substr(c2 + 1, str.size()).data()) / 255.0f,
			1.0f
		);
	};

	QMaterial* baseMaterial = new QMaterial();
	baseMaterial->addParameter(new QParameter("diffuse", ColourFromXmlAttribute(materialXml.attribute("diffuse"))));
	baseMaterial->addParameter(new QParameter("selfillum", ColourFromXmlAttribute(materialXml.attribute("selfillum"))));
	baseMaterial->addParameter(new QParameter("shininess", materialXml.attribute("shininess").as_float()));
	baseMaterial->addParameter(new QParameter("shin_strength", materialXml.attribute("shin_strength").as_float()));
	baseMaterial->addParameter(new QParameter("shin_strength", materialXml.attribute("shin_strength").as_float()));
	baseMaterial->addParameter(new QParameter("spec_color", ColourFromXmlAttribute(materialXml.attribute("spec_color"))));
	baseMaterial->addParameter(new QParameter("opacity", materialXml.attribute("opacity").as_float()));

	/*
	baseMaterial->addParameter(
		new QParameter("basetexture",

	))
	*/;

	return {};
}

std::unique_ptr<Qt3DRender::QShaderProgram>&& MaterialFactory::CreateShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFilename)
{
	auto shaderProgram = std::make_unique<QShaderProgram>();
	shaderProgram->setFormat(QShaderProgram::GLSL);

	const auto LoadShaderSource = [](const char* sourceFilename)
	{
		const std::filesystem::path vertexShaderFilename = gShaderFolder / sourceFilename;
		std::ifstream baseShaderFile(vertexShaderFilename, std::ios::binary | std::ios::end);

		QByteArray fileContents;
		fileContents.resize(baseShaderFile.tellg());
		baseShaderFile.seekg(0, std::ios::beg).read(fileContents.data(), fileContents.size());

		return fileContents;
	};

	shaderProgram->setVertexShaderCode(LoadShaderSource(vertexShaderFileName));
	shaderProgram->setFragmentShaderCode(LoadShaderSource(fragmentShaderFilename));

	if (shaderProgram->status() != QShaderProgram::Ready)
	{
		throw std::runtime_error("Failed to compile shader");
	}

	return std::move(shaderProgram);
}
