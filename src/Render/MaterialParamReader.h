#pragma once

#include <QVector3D>
#include <QColor>
#include <QQuaternion>
#include <QAbstractTexture>

#include "Utilities/configfile.h"

namespace Qt3DRender {
	class QMaterial;
	class QAbstractTexture;
}

namespace MGF {
	class File;
}

namespace pugi {
	class xml_node;
}

namespace MGF::Render {

	enum class ShadingType : uint8_t
	{
		None,
		Flat,
		MaterialOnly,
		Gouraud,
		Translucent
	};

	enum class BlendingType : uint8_t
	{
		Additive,
		Alpha,
		Normal,
		Subtractive
	};

	class IMaterialParamReader
	{
	public:
		virtual float ReadFloat(const char* name, const char* altName = nullptr) = 0;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) = 0;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) = 0;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) = 0;
		virtual Qt3DRender::QAbstractTexture* ReadTexture(const char* name, const char* altName = nullptr) = 0;

		inline static QVector3D StrToVector(const std::string_view str)
		{
			const auto c1 = str.find(',', 0);
			const auto c2 = str.find(',', c1 + 1);

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

		inline static ShadingType ShadingTypeFromString(const std::string_view str)
		{
			if (str == "material_only")
				return ShadingType::MaterialOnly;
			else if (str == "gouraud")
				return ShadingType::Gouraud;
			else if (str == "flat")
				return ShadingType::Flat;
			else if (str == "translucent")
				return ShadingType::Translucent;
			else if (str == "none")
				return ShadingType::None;
			else
				throw std::runtime_error("Unsupported shading type");
		}

		inline static BlendingType BlendingTypeFromString(const std::string_view str)
		{
			if (str == "additive")
				return BlendingType::Additive;
			else if (str == "alpha")
				return BlendingType::Alpha;
			else if (str == "normal")
				return BlendingType::Normal;
			else if (str == "subtractive")
				return BlendingType::Subtractive;
			else
				throw std::runtime_error("Unsupported blending type");
		}
		
	protected:
		virtual const char* SelectParam(const char* a, const char* b) const = 0;
	};

	class MGFMaterialFileReader final : public IMaterialParamReader
	{
	public:
		MGFMaterialFileReader(const MGF::File& materialFile, const MGF::File& parentFile);

		virtual float ReadFloat(const char* name, const char* altName = nullptr) override;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) override;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) override;
		virtual Qt3DRender::QAbstractTexture* ReadTexture(const char* name, const char* altName = nullptr) override;

	protected:
		virtual const char* SelectParam(const char* a, const char* b) const override;

	private:
		ConVariables ConfigVariables;
		const MGF::File& ParentFile;
	};

	class MGFMaterialNodeXMLReader final : public IMaterialParamReader
	{
	public:
		MGFMaterialNodeXMLReader(const pugi::xml_node& materialNode, const MGF::File& mgmodelFile) : MaterialNode{ materialNode }, MGModelFile{ mgmodelFile } {}

		virtual float ReadFloat(const char* name, const char* altName = nullptr) override;
		virtual bool ReadBool(const char* name, const char* altName = nullptr) override;
		virtual QVector3D ReadVector3(const char* name, const char* altName = nullptr) override;
		virtual std::string_view ReadString(const char* name, const char* altName = nullptr) override;
		virtual Qt3DRender::QAbstractTexture* ReadTexture(const char* name, const char* altName = nullptr) override;

	protected:
		virtual const char* SelectParam(const char* a, const char* b) const override;

	private:
		const pugi::xml_node& MaterialNode;
		const MGF::File& MGModelFile;
	};
}