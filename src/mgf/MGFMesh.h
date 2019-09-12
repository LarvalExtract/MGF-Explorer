#pragma once

#include "MGFTreeNode.h"
#include "MGFMaterial.h"
#include "../renderer/VertexArray.h"

#include <fstream>
#include <memory>
#include <pugixml-1.9/src/pugixml.hpp>
#include <glm/mat4x4.hpp>

class MGFMesh
{
public:
	MGFMesh(const MGFTreeNode& vbNode, const MGFTreeNode& ibNode, const pugi::xml_node& meshxml, const MGFMaterial* mat);

	glm::mat4x4 transform;
	VertexArray vao;
	const MGFMaterial* mat;
	bool bSelected;
	bool bScaleTexCoords;

	std::uint32_t numVertices;
	std::uint32_t stride;
	std::uint32_t flags;

	friend bool operator<(const MGFMesh& lhs, const MGFMesh& rhs);

private:
	VertexBuffer LoadVertexBuffer(const MGFTreeNode& vbNode);
	IndexBuffer LoadIndexBuffer(const MGFTreeNode& ibNode, const char* typeAttribute);
};