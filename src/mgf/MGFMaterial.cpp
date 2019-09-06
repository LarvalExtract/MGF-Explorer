#include "MGFMaterial.h"
#include "MGFArchive.h"

#include "../application.h"

#include <sstream>

MGFMaterial::MGFMaterial(MaterialType type) :
	type(type)
{
	switch (type)
	{
	case MaterialType::Animated:		break;
	case MaterialType::Base:			shader = Application::material_base_shader; break;
	case MaterialType::ColorShift:		shader = Application::material_base_shader; break;
	case MaterialType::Complex:			shader = Application::material_complex_shader; break;
	case MaterialType::Detail:			shader = Application::material_base_shader; break;
	case MaterialType::Distort:			break;
	case MaterialType::Env:				shader = Application::material_base_shader; break;
	case MaterialType::SpecularAlpha:	break;
	case MaterialType::SpecularMask:	shader = Application::material_base_shader; break;
	default:							shader = Application::shader_default; break;
	}
}

MGFMaterialBase::MGFMaterialBase(const MGFTreeNode& node, const pugi::xml_node& material_base) :
	MGFMaterial(MaterialType::Base),
	basetexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_base.child("basetexture").attribute("filename").as_string()))))
{
	//diffuse = GetVec3Attribute(material_base.attribute("diffuse"));
	//selfillum = GetVec3Attribute(material_base.attribute("selfillum"));
	//shininess = material_base.attribute("shininess").as_float();
	//shin_strength = material_base.attribute("shin_strength").as_float();
	//spec_color = GetVec3Attribute(material_base.attribute("spec_color"));
	//opacity = material_base.attribute("opacity").as_float();
	two_sided = material_base.attribute("two_sided").as_bool();
}

MGFMaterialComplex::MGFMaterialComplex(const MGFTreeNode& node, const pugi::xml_node& material_complex) :
	MGFMaterial(MaterialType::Complex),
	basetexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_complex.child("basetexture").attribute("filename").as_string())))),
	multitexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_complex.child("multitexture").attribute("filename").as_string()))))
{
	two_sided = material_complex.attribute("two_sided").as_bool();
}

MGFMaterialSpecularMask::MGFMaterialSpecularMask(const MGFTreeNode& node, const pugi::xml_node& material_specular_mask) :
	MGFMaterial(MaterialType::SpecularMask),
	basetexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_specular_mask.child("basetexture").attribute("filename").as_string()))))
{
	two_sided = material_specular_mask.attribute("two_sided").as_bool();
}

MGFMaterialDetail::MGFMaterialDetail(const MGFTreeNode& node, const pugi::xml_node& material_detail) :
	MGFMaterial(MaterialType::Detail),
	basetexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_detail.child("basetexture").attribute("filename").as_string()))))
{
	two_sided = material_detail.attribute("two_sided").as_bool();
}

MGFMaterialColorShift::MGFMaterialColorShift(const MGFTreeNode& node, const pugi::xml_node& material_color_shift) :
	MGFMaterial(MaterialType::ColorShift),
	basetexture(*Application::GetTexture(*node.GetRelativeNode(std::string(material_color_shift.child("basetexture").attribute("filename").as_string()))))
{
	two_sided = material_color_shift.attribute("two_sided").as_bool();
}

glm::vec3 MGFMaterial::GetVec3Attribute(const pugi::xml_attribute& attrib)
{
	std::stringstream ss;

	return glm::vec3(0);
}


void MGFMaterial::Apply(const glm::mat4& mvpMatrix) const
{
	shader->Use();
	shader->SetUniformMat4("u_mvpMatrix", mvpMatrix);
	shader->SetUniformBool("u_two_sided", two_sided);
}

void MGFMaterialBase::Apply(const glm::mat4& mvpMatrix) const
{
	MGFMaterial::Apply(mvpMatrix);
	basetexture.Bind(0);
}

void MGFMaterialComplex::Apply(const glm::mat4& mvpMatrix) const
{
	MGFMaterial::Apply(mvpMatrix);
	basetexture.Bind(0);
	multitexture.Bind(1);
}



void MGFMaterialSpecularMask::Apply(const glm::mat4& mvpMatrix) const
{
	MGFMaterial::Apply(mvpMatrix);
	basetexture.Bind(0);
}

void MGFMaterialDetail::Apply(const glm::mat4& mvpMatrix) const
{
	MGFMaterial::Apply(mvpMatrix);
	basetexture.Bind(0);
}

void MGFMaterialColorShift::Apply(const glm::mat4& mvpMatrix) const
{
	MGFMaterial::Apply(mvpMatrix);
	basetexture.Bind(0);
}
