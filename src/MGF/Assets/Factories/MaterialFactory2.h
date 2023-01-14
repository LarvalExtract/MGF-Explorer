#pragma once

#include "MGF/File.h"

#include <pugixml-1.10/src/pugixml.hpp>
#include <memory>

namespace Qt3DRender
{
	class QMaterial;
	class QEffect;
	class QTechnique;
	class QRenderPass;
	class QShaderProgram;
}

namespace MA
{
	enum class MaterialType : unsigned int
	{
		Base,
		Solid,
		Complex,
		ColorShift,
		SpecularMask,
		SpecularAlpha,
		Env,
		Detail,
		Distort,
		Animated,
		DiffuseShift
	};

	struct Material
	{
		MA::MaterialType type;
		Qt3DRender::QMaterial* material = nullptr;
	};

	class MaterialFactory final
	{
	public:
		MaterialFactory();
		~MaterialFactory();

		MaterialFactory(const MaterialFactory&) = delete;
		MaterialFactory(MaterialFactory&&) = delete;
		MaterialFactory& operator=(const MaterialFactory&) = delete;
		MaterialFactory& operator=(MaterialFactory&&) = delete;

		Material CreateMaterial(const pugi::xml_node& materialXml, const MGF::File& sourceFile);
		Material CreateMaterial(const MGF::File& matFile, const MGF::File& sourceFile);

	private:
		Qt3DRender::QMaterial* CreateMaterialBase(const pugi::xml_node& materialXml);

		std::unique_ptr<Qt3DRender::QShaderProgram>&& CreateShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFilename);

		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderSolid;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderBase;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderComplex;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderColorShift;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderSpecularMask;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderSpecularAlpha;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderEnv;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderDetail;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderDistort;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderAnimated;
		std::unique_ptr<Qt3DRender::QShaderProgram> mShaderDiffuseShift;
	};
}