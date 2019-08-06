#include "MGFMesh.h"

#include <stdint.h>

enum VertexFileOffset
{
	VertexCount = 84,
	Flags = 88,
	VertexBufferSize = 120,
	VertexData = 124
};

enum IndexFileOffset
{
	IndexCount = 88,
	IndexBufferSize = 96,
	IndexData = 100
};

MGFMesh::MGFMesh(const MGFTreeNode& vbNode, const MGFTreeNode& ibNode, std::ifstream& fileStream, const pugi::xml_node& meshxml)
{
	VertexBuffer vb = LoadVertexBuffer(vbNode, fileStream);
	IndexBuffer ib = LoadIndexBuffer(ibNode, fileStream, meshxml.attribute("type").as_string());

	vao.SetVertexBuffer(vb);
	vao.SetIndexBuffer(ib);
}

VertexBuffer MGFMesh::LoadVertexBuffer(const MGFTreeNode& vbNode, std::ifstream& fileStream)
{
	std::vector<char> buffer(vbNode.FileLength());
	fileStream.seekg(vbNode.FileOffset());
	fileStream.read(buffer.data(), vbNode.FileLength());

	std::uint32_t numVertices = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::VertexCount]);
	std::uint32_t flags = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::Flags]);
	std::uint32_t size = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::VertexBufferSize]) - 8;

	int stride = size / numVertices;

	BufferLayout layout;

	switch (flags)
	{
	case 0x00000101:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec2s, "in_texCoord", true }
		};
		break;

	case 0x00000301:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2s, "in_texCoord", true }
		};
		break;

	case 0x00000309:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec2u, "" },
			{ GLSLType::Vec2s, "in_texCoord", true }
		};
		break;

	case 0x00000501:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec2s, "in_texCoord", true },
		};
		break;

	case 0x00001101:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	case 0x00002101:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	case 0x00080301:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	case 0x00100301:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec4i, "" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	case 0x00100501:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec4i, "" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	case 0x00180301:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec4i, "" },
			{ GLSLType::Int, "" },
			{ GLSLType::Vec2f, "in_texCoord" }
		};
		break;

	default:
		layout = {
			{ GLSLType::Vec3f, "in_position" },
			{ GLSLType::Vec2u, "in_texCoord", true }
		};
		break;
	}

	layout.stride = stride;

	return VertexBuffer(&buffer[VertexFileOffset::VertexData], size, layout);
}

IndexBuffer MGFMesh::LoadIndexBuffer(const MGFTreeNode& ibNode, std::ifstream& fileStream, const char* typeAttribute)
{
	std::vector<char> buffer(ibNode.FileLength());
	fileStream.seekg(ibNode.FileOffset(), std::ios::beg);
	fileStream.read(buffer.data(), ibNode.FileLength());

	std::uint32_t numIndices = *reinterpret_cast<std::uint32_t*>(&buffer[IndexFileOffset::IndexCount]);
	std::uint32_t size = *reinterpret_cast<std::uint32_t*>(&buffer[IndexFileOffset::IndexBufferSize]) - 8;

	PrimitiveType type = (std::strcmp(typeAttribute, "indexedlist") == 0) ? PrimitiveType::Triangles : PrimitiveType::TriangleStrip;

	return IndexBuffer(reinterpret_cast<unsigned short*>(&buffer[IndexFileOffset::IndexData]), numIndices, type);
}
