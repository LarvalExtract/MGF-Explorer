#include "MGFMesh.h"
#include "MGFArchive.h"

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

MGFMesh::MGFMesh(const MGFTreeNode& vbNode, const MGFTreeNode& ibNode, const pugi::xml_node& meshxml, const MGFMaterial* mat) :
	vao(LoadVertexBuffer(vbNode), LoadIndexBuffer(ibNode, meshxml.attribute("type").as_string())),
	mat(mat),
	bSelected(false),
	transform(glm::mat4(1.0f))
{

}

MGFMesh::~MGFMesh()
{
	
}

VertexBuffer MGFMesh::LoadVertexBuffer(const MGFTreeNode& vbNode)
{
	std::vector<char> buffer(vbNode.FileLength());
	vbNode.archive.FileStream().seekg(vbNode.FileOffset());
	vbNode.archive.FileStream().read(buffer.data(), vbNode.FileLength());

	numVertices = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::VertexCount]);
	flags = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::Flags]);
	std::uint32_t size = *reinterpret_cast<std::uint32_t*>(&buffer[VertexFileOffset::VertexBufferSize]) - 8;

	stride = size / numVertices;

	BufferLayout layout;

	bScaleTexCoords = true;
	std::uint32_t uvOffset;

	switch (flags)
	{
	case 0x00000101:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int }
		};
		break;

	case 0x00000301:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 16;
		break;

	case 0x00000309:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec2u },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 20;
		break;

	case 0x00000501:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int, },
			{ GLSLType::Vec2s, "in_texCoord", 1, true}
		};
		uvOffset = 12;
		break;

	case 0x00001101:
		bScaleTexCoords = false;
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int },
			{ GLSLType::Vec2f, "in_texCoord", 1 }
		};
		uvOffset = 16;
		break;

	case 0x00002101:
		bScaleTexCoords = false;
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int },
			{ GLSLType::Vec2f, "in_texCoord", 1 }
		};
		uvOffset = 16;
		break;

	case 0x00080301:
		bScaleTexCoords = false;
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Int },
			{ GLSLType::Vec2f, "in_texCoord", 1 },
			{ GLSLType::Vec4f, "" }
		};
		uvOffset = 16;
		break;

	case 0x00100301:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec4i },
			{ GLSLType::Int },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 32;
		break;

	case 0x00100501:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec4i },
			{ GLSLType::Int },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 32;
		break;

	case 0x00180301:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec4i },
			{ GLSLType::Int },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 32;
		break;

	default:
		layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec2s, "in_texCoord", 1, true }
		};
		uvOffset = 12;
		break;
	}

	layout.stride = stride;

	//if (bScaleTexCoords == true)
	//	for (std::size_t i = 0; i < numVertices; i++)
	//	{
	//		std::int16_t* u = reinterpret_cast<std::int16_t*>(&buffer[VertexFileOffset::VertexData + ((i * stride) + uvOffset)]);
	//		std::int16_t* v = reinterpret_cast<std::int16_t*>(&buffer[VertexFileOffset::VertexData + ((i * stride) + (uvOffset + 2))]);
	//
	//		*u <<= 2;
	//		*u <<= 2;
	//	}

	return VertexBuffer(&buffer[VertexFileOffset::VertexData], size, layout);
}

IndexBuffer MGFMesh::LoadIndexBuffer(const MGFTreeNode& ibNode, const char* typeAttribute)
{
	std::vector<char> buffer(ibNode.FileLength());
	ibNode.archive.FileStream().seekg(ibNode.FileOffset(), std::ios::beg);
	ibNode.archive.FileStream().read(buffer.data(), ibNode.FileLength());

	std::uint32_t numIndices = *reinterpret_cast<std::uint32_t*>(&buffer[IndexFileOffset::IndexCount]);
	std::uint32_t size = *reinterpret_cast<std::uint32_t*>(&buffer[IndexFileOffset::IndexBufferSize]) - 8;

	PrimitiveType type;
	if (std::strcmp(typeAttribute, "indexedlist") == 0)
		type = PrimitiveType::Triangles;
	else if (std::strcmp(typeAttribute, "indexedstrip") == 0)
		type = PrimitiveType::TriangleStrip;
	else
		type = PrimitiveType::Lines;
		
	return IndexBuffer(reinterpret_cast<unsigned short*>(&buffer[IndexFileOffset::IndexData]), numIndices, type);
}

bool operator<(const MGFMesh& lhs, const MGFMesh& rhs)
{
	return static_cast<int>(lhs.mat->type) < static_cast<int>(rhs.mat->type);
}