#pragma once

#include <QVector3D>
#include <QColor>
#include <QQuaternion>
#include <QMaterial>
#include <QAbstractTexture>
#include <QBlendEquation>
#include <QBlendEquationArguments>
#include <QCullFace>

#include "Utilities/configfile.h"

class MGFFile;

namespace pugi {
	class xml_node;
}

namespace MGF::Render {

	struct TextureParams
	{
		const std::string_view Filepath;
		const Qt3DRender::QTextureWrapMode::WrapMode TilingX, TilingY;
		const bool bMipMapping;
		const float MipBias;
		const Qt3DRender::QAbstractTexture::Filter MinificationFilter;
		const Qt3DRender::QAbstractTexture::Filter MagnificationFilter;
	};

	class IMaterialParamReader
	{
	public:
		IMaterialParamReader(const MGFFile& parentFile) : ParentFile(parentFile) {}

		virtual Qt3DRender::QAbstractTexture* ReadTexture(const char* name, const char* altName = nullptr);
		virtual QPair<Qt3DRender::QBlendEquation*, Qt3DRender::QBlendEquationArguments*> ReadBlendingMode(const char* name, const char* altName = nullptr);
		virtual Qt3DRender::QCullFace* ReadCullingMode(const char* name, const char* altName = nullptr);

		virtual float ReadFloat(const char* name, const char* altName = nullptr) = 0;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) = 0;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) = 0;
		virtual QColor ReadColour(const char* name, const char* altName = nullptr) = 0;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) = 0;
		virtual std::string_view ReadMaterialType() = 0;

		inline static QVector3D StrToVector(const std::string_view str)
		{
			const size_t c1 = str.find(',', 0);
			const size_t c2 = str.find(',', c1 + 1);

			return QVector3D(
				std::stof(str.substr(0, c1).data()),
				std::stof(str.substr(c1 + 1, c2).data()),
				std::stof(str.substr(c2 + 1, str.size()).data())
			);
		}

		inline static QColor StringToColor(const std::string_view str)
		{
			const QVector3D vec = StrToVector(str);
			return QColor::fromRgbF(vec.x() / 255.0f, vec.y() / 255.0f, vec.z() / 255.0f);
		}

		inline static QQuaternion StringToQuat(const std::string_view axis, const std::string_view angle)
		{
			return QQuaternion::fromAxisAndAngle(StrToVector(axis), std::stof(angle.data()));
		}
		
	protected:
		virtual const char* SelectParam(const char* a, const char* b) const = 0;
		virtual TextureParams ReadTextureParams(const char* name, const char* altName = nullptr) = 0;

		const MGFFile& ParentFile;
	};

	/*
		MGFMaterialFileReader reads parameters from .mat files, which are just config (.ini) files
	*/
	class MGFMaterialFileReader final : public IMaterialParamReader
	{
	public:
		MGFMaterialFileReader(const MGFFile& materialFile, const MGFFile& parentFile)
			: IMaterialParamReader(parentFile)
			, ConfigVariables{ ConfigFile(&materialFile)["material"] }
		{

		}

		virtual float ReadFloat(const char* name, const char* altName = nullptr) override;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) override;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) override;
		virtual QColor ReadColour(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadMaterialType() override;

	protected:
		virtual const char* SelectParam(const char* a, const char* b) const override;
		virtual TextureParams ReadTextureParams(const char* name, const char* altName = nullptr) override;

	private:
		ConVariables ConfigVariables;
	};

	/*
		MGFMaterialNodeXMLReader reads parameters from the <material> nodes found at the start of .mgmodel files
	*/
	class MGFMaterialNodeXMLReader final : public IMaterialParamReader
	{
	public:
		MGFMaterialNodeXMLReader(const pugi::xml_node& materialNode, const MGFFile& mgmodelFile)
			: IMaterialParamReader(mgmodelFile)
			, MaterialNode{ materialNode }
		{
		}

		virtual float ReadFloat(const char* name, const char* altName = nullptr) override;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) override;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) override;
		virtual QColor ReadColour(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadMaterialType() override;

	protected:
		virtual const char* SelectParam(const char* a, const char* b) const override;
		virtual TextureParams ReadTextureParams(const char* name, const char* altName = nullptr) override;

	private:
		const pugi::xml_node& MaterialNode;
	};
}