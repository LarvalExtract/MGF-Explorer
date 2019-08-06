#pragma once

#include "MGFTreeNode.h"
#include "../renderer/VertexArray.h"

#include <fstream>
#include <pugixml-1.9/src/pugixml.hpp>

class MGFMesh
{
public:
	MGFMesh(const MGFTreeNode& vbNode, const MGFTreeNode& ibNode, std::ifstream& fileStream, const pugi::xml_node& meshxml);

private:
	VertexArray vao;
	
	VertexBuffer LoadVertexBuffer(const MGFTreeNode& vbNode, std::ifstream& fileStream);
	IndexBuffer LoadIndexBuffer(const MGFTreeNode& ibNode, std::ifstream& fileStream, const char* typeAttribute);
};