#include "MaterialParamReader.h"
#include "TextureLibrary.h"
#include "MGF/File.h"
#include "MGF/Archive.h"

#include <pugixml-1.10/src/pugixml.hpp>

using namespace Qt3DRender;

namespace MGF::Render {

	Qt3DRender::QAbstractTexture* IMaterialParamReader::ReadTexture(const char* name, const char* altName)
	{
		TextureParams textureParams = ReadTextureParams(name, altName);

		Qt3DRender::QAbstractTexture* texture = nullptr;
		if (const MGF::File* textureFile = ParentFile.FindRelativeItem(textureParams.Filepath))
		{
			texture = MGF::Render::TextureLibrary::Get().GetTexture(*textureFile);

			texture->setMinificationFilter(textureParams.MinificationFilter);
			texture->setMagnificationFilter(textureParams.MagnificationFilter);
			texture->setWrapMode(QTextureWrapMode(textureParams.TilingX, textureParams.TilingY, QTextureWrapMode::ClampToEdge));
			texture->setGenerateMipMaps(false);
		}
		else
		{
			qDebug() << "Could not find texture file " << textureParams.Filepath.data();
		}

		return texture;
	}

	QPair<Qt3DRender::QBlendEquation*, Qt3DRender::QBlendEquationArguments*> IMaterialParamReader::ReadBlendingMode(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);

		QBlendEquation* blendEquation = new QBlendEquation();
		QBlendEquationArguments* blendState = new QBlendEquationArguments();

		if (const std::string_view blendingModeStr = ReadString(name); 
			blendingModeStr == "additive")
		{
			blendEquation->setBlendFunction(QBlendEquation::Add);
			blendState->setSourceRgb(QBlendEquationArguments::One);
			blendState->setDestinationRgb(QBlendEquationArguments::One);
		}
		else if (blendingModeStr == "normal")
		{
			blendEquation->setBlendFunction(QBlendEquation::Add);
			blendState->setSourceRgb(QBlendEquationArguments::One);
			blendState->setDestinationRgb(QBlendEquationArguments::Zero);
		}
		else if (blendingModeStr == "alpha")
		{
			blendEquation->setBlendFunction(QBlendEquation::Add);
			blendState->setSourceRgb(QBlendEquationArguments::SourceAlpha);
			blendState->setDestinationRgb(QBlendEquationArguments::OneMinusSourceAlpha);
		}
		else
		{
			qDebug() << "Unimplemented blending mode: " << blendingModeStr.data();
		}

		return qMakePair(blendEquation, blendState);
	}

	QCullFace* IMaterialParamReader::ReadCullingMode(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);

		QCullFace* cullMode = new QCullFace;
		cullMode->setMode(ReadBool(name) ? QCullFace::NoCulling : QCullFace::Back);
		return cullMode;
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

	QColor MGFMaterialFileReader::ReadColour(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return IMaterialParamReader::StringToColor(ConfigVariables.at(name));
	}

	std::string_view MGFMaterialFileReader::ReadString(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return ConfigVariables.at(name);
	}

	std::string_view MGFMaterialFileReader::ReadMaterialType()
	{
		return ReadString("type");
	}

	const char* MGFMaterialFileReader::SelectParam(const char* a, const char* b) const
	{
		return ConfigVariables.contains(a) ? a : ConfigVariables.contains(b) ? b : nullptr;
	}

	TextureParams MGFMaterialFileReader::ReadTextureParams(const char* name, const char* altName) 
	{
		name = SelectParam(name, altName);

		const std::string tilingAttribute = std::string(name) + "_tiling";
		const std::string mipmappingAttribute = std::string(name) + "_mipmapping";
		const std::string mipmapBiasAttribute = std::string(name) + "_mipmap_bias";

		return TextureParams{
			.Filepath = ReadString(name),
			.TilingX = QTextureWrapMode::Repeat, 
			.TilingY = QTextureWrapMode::Repeat,
			.bMipMapping = ReadBool(mipmappingAttribute.c_str()),
			.MipBias = ReadFloat(mipmapBiasAttribute.c_str()),
			.MinificationFilter = QAbstractTexture::Filter::Linear,
			.MagnificationFilter = QAbstractTexture::Filter::Linear,
		};
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

	QColor MGFMaterialNodeXMLReader::ReadColour(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return IMaterialParamReader::StringToColor(MaterialNode.attribute(name).as_string());
	}

	std::string_view MGFMaterialNodeXMLReader::ReadString(const char* name, const char* altName)
	{
		name = SelectParam(name, altName);
		return MaterialNode.attribute(name).as_string();
	}

	std::string_view MGFMaterialNodeXMLReader::ReadMaterialType()
	{
		const std::string_view name = MaterialNode.name();
		return name.substr(9, name.length());
	}

	const char* MGFMaterialNodeXMLReader::SelectParam(const char* a, const char* b) const
	{
		if (a && !MaterialNode.attribute(a).empty())
		{
			return a;
		}
		else if (b && !MaterialNode.attribute(b).empty())
		{
			return b;
		}
		return nullptr;
	}
	TextureParams MGFMaterialNodeXMLReader::ReadTextureParams(const char* name, const char* altName)
	{
		if (altName && !MaterialNode.child(altName).empty())
		{
			name = altName;
		}
		
		if (const pugi::xml_node& textureNode = MaterialNode.child(name); !textureNode.empty())
		{
			return TextureParams{
				.Filepath = textureNode.attribute("filename").as_string(),
				.TilingX = QTextureWrapMode::Repeat,
				.TilingY = QTextureWrapMode::Repeat,
				.bMipMapping = textureNode.attribute("mipmapping").as_bool(),
				.MipBias = textureNode.attribute("mipmap_bias").as_float(),
				.MinificationFilter = QAbstractTexture::Filter::Linear,
				.MagnificationFilter = QAbstractTexture::Filter::Linear,
			};
		}
		else
		{
			qDebug() << "Could not find texture " << name << " in " << MaterialNode.name() << ParentFile;
		}
	}
}


