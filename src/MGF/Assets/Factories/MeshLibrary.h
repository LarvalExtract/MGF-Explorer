#pragma once

#include "MGF/MGFFile.h"

#include <QVector3D>
#include <QGeometryRenderer>

#include <unordered_map>
#include <pugixml-1.10/src/pugixml.hpp>

namespace Qt3DCore
{
	class QGeometry;
	class QBuffer;
}

namespace Qt3DRender
{
	class QGeometryRenderer;
}

namespace MA
{
	class MeshLibrary final
	{
	public:
		Qt3DRender::QGeometryRenderer* CreateMesh(const pugi::xml_node& meshxml, const MGFFile& sourceFile);
		Qt3DRender::QGeometryRenderer* CreateMesh(const MGFFile& meshFile, const MGFFile& sourceFile);

	private:
		Qt3DRender::QGeometryRenderer* CreateMesh(const std::string_view verticesFilename, const std::string_view indicesFilename, const std::string_view material, const MGFFile& sourceFile, Qt3DRender::QGeometryRenderer::PrimitiveType primitiveType);

		int LoadVertexBuffer(const MGFFile& verticesFile, Qt3DCore::QGeometry& geom, QVector3D& minExtents, QVector3D& maxExtents);
		void LoadIndexBuffer(const MGFFile& indicesFile, Qt3DCore::QGeometry& geom);

		void SetGeometryAttributesFromFlags(uint32_t flags, uint32_t count, uint32_t stride, Qt3DCore::QGeometry& geom, Qt3DCore::QBuffer* vertexBuffer);

		std::unordered_map<uint64_t, Qt3DRender::QGeometryRenderer*> mGeometryMap;
	};
}