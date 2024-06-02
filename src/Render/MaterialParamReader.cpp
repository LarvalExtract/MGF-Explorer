#include "MaterialParamReader.h"
#include "MGF/File.h"
#include "MGF/Archive.h"
#include "MGF/Assets/Factories/TextureLibrary.h"

#include <pugixml-1.10/src/pugixml.hpp>

namespace MGF::Render {

	MGFMaterialFileReader::MGFMaterialFileReader(const MGF::File& materialFile, const MGF::File& parentFile) :
		ParentFile{parentFile}
	{
		ConfigFile cfg(&materialFile);
		ConfigVariables = cfg["material"];
	}

	float MGFMaterialFileReader::ReadFloat(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return std::stof(ConfigVariables[name]);
	}

	bool MGFMaterialFileReader::ReadBool(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return ConfigVariables[name][0] == 't' || ConfigVariables[name][0] == 'T';
	}

	QVector3D MGFMaterialFileReader::ReadVector3(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return IMaterialParamReader::StrToVector(ConfigVariables.at(name));
	}

	std::string_view MGFMaterialFileReader::ReadString(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return ConfigVariables.at(name);
	}

	Qt3DRender::QAbstractTexture* MGFMaterialFileReader::ReadTexture(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		const std::string_view texturePath = ConfigVariables.at(name);
		const std::string tilingAttribute = std::string(name) + "_tiling";

		Qt3DRender::QAbstractTexture* texture = nullptr;

		if (const MGF::File* textureFile = ParentFile.FindRelativeItem(texturePath))
		{
			texture = MGF::Render::TextureLibrary::GetTexture(textureFile);

			texture->setMinificationFilter(Qt3DRender::QAbstractTexture::Filter::Linear);
			texture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Filter::Linear);

			const std::string_view tiling = ConfigVariables.at(tilingAttribute);
			Qt3DRender::QTextureWrapMode wrapMode;
			wrapMode.setX(tiling.contains("U_WRAP") ? Qt3DRender::QTextureWrapMode::Repeat : Qt3DRender::QTextureWrapMode::ClampToEdge);
			wrapMode.setY(tiling.contains("V_WRAP") ? Qt3DRender::QTextureWrapMode::Repeat : Qt3DRender::QTextureWrapMode::ClampToEdge);
			texture->setWrapMode(wrapMode);

			texture->setGenerateMipMaps(false);
		}

		return texture;
	}

	const char* MGFMaterialFileReader::SelectParam(const char* a, const char* b) const
	{
		return ConfigVariables.contains(a) ? a : ConfigVariables.contains(b) ? b : nullptr;
	}

	float MGFMaterialNodeXMLReader::ReadFloat(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return MaterialNode.attribute(name).as_float();
	}

	bool MGFMaterialNodeXMLReader::ReadBool(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return MaterialNode.attribute(name).as_bool();
	}

	QVector3D MGFMaterialNodeXMLReader::ReadVector3(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return IMaterialParamReader::StrToVector(MaterialNode.attribute(name).as_string());
	}

	std::string_view MGFMaterialNodeXMLReader::ReadString(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return MaterialNode.attribute(name).as_string();
	}

	Qt3DRender::QAbstractTexture* MGFMaterialNodeXMLReader::ReadTexture(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		const pugi::xml_node& textureNode = MaterialNode.child(name);
		const std::string_view texturePath = textureNode.attribute("filename").as_string();

		Qt3DRender::QAbstractTexture* texture = nullptr;

		if (const MGF::File* textureFile = MGModelFile.FindRelativeItem(texturePath))
		{
			texture = MGF::Render::TextureLibrary::GetTexture(textureFile);

			Qt3DRender::QAbstractTexture::Filter filter;
			const std::string_view filterAttribute = textureNode.attribute("filter").as_string();
			filter = filterAttribute == "FILTER_LINEAR" ? Qt3DRender::QAbstractTexture::Filter::Linear : Qt3DRender::QAbstractTexture::Filter::Nearest;
			texture->setMinificationFilter(filter);
			texture->setMagnificationFilter(filter);

			const std::string_view tilingAttribute = textureNode.attribute("tiling").as_string();
			Qt3DRender::QTextureWrapMode wrapMode;
			wrapMode.setX(tilingAttribute.contains("U_WRAP") ? Qt3DRender::QTextureWrapMode::Repeat : Qt3DRender::QTextureWrapMode::ClampToEdge);
			wrapMode.setY(tilingAttribute.contains("V_WRAP") ? Qt3DRender::QTextureWrapMode::Repeat : Qt3DRender::QTextureWrapMode::ClampToEdge);
			texture->setWrapMode(wrapMode);

			texture->setGenerateMipMaps(false);
		}

		return texture;
	}
	const char* MGFMaterialNodeXMLReader::SelectParam(const char* a, const char* b) const
	{
		return !MaterialNode.attribute(a).empty() ? a : !MaterialNode.attribute(a).empty() ? b : nullptr;
	}
}


