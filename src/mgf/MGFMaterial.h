#pragma once

#include "MGFTreeNode.h"
#include "MATexture.h"
#include "../renderer/Shader.h"

#include <pugixml-1.9/src/pugixml.hpp>
#include <glm/vec3.hpp>

#include <memory>

enum class MaterialType
{
	Animated,
	Base,
	ColorShift,
	Complex,
	Detail,
	Distort,
	Env,
	SpecularAlpha,
	SpecularMask,
	Unknown
};

class MGFMaterial
{
public:
	MGFMaterial(MaterialType type = MaterialType::Unknown);

	virtual void Apply(const glm::mat4& mvpMatrix) const;

	const MaterialType type;
	std::shared_ptr<ShaderProgram> shader;

protected:
	static glm::vec3 GetVec3Attribute(const pugi::xml_attribute& attrib);

	bool two_sided;
};

class MGFMaterialBase : public MGFMaterial
{
public:
	MGFMaterialBase(const MGFTreeNode& node, const pugi::xml_node& material_base);

	void Apply(const glm::mat4& mvpMatrix) const override;

private:
	const MATexture& basetexture;
};

class MGFMaterialComplex : public MGFMaterial
{
public:
	MGFMaterialComplex(const MGFTreeNode& node, const pugi::xml_node& material_complex);

	void Apply(const glm::mat4& mvpMatrix) const override;

private:
	const MATexture& basetexture;
	const MATexture& multitexture;
};

class MGFMaterialSpecularMask : public MGFMaterial
{
public:
	MGFMaterialSpecularMask(const MGFTreeNode& node, const pugi::xml_node& material_specular_mask);

	void Apply(const glm::mat4& mvpMatrix) const override;

private:
	const MATexture& basetexture;
};

class MGFMaterialDetail : public MGFMaterial
{
public:
	MGFMaterialDetail(const MGFTreeNode& node, const pugi::xml_node& material_detail);

	void Apply(const glm::mat4& mvpMatrix) const override;

private:
	const MATexture& basetexture;
};

class MGFMaterialColorShift : public MGFMaterial
{
public:
	MGFMaterialColorShift(const MGFTreeNode& node, const pugi::xml_node& material_color_shift);

	void Apply(const glm::mat4& mvpMatrix) const override;

private:
	const MATexture& basetexture;
};

class MGFMaterialAnimated : public MGFMaterial
{
public:

private:
	int animMaxFrameNum;
};

