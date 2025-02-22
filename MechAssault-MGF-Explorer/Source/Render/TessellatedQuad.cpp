#include "TessellatedQuad.h"

#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QGeometry>

namespace TessellatedQuadRendererPrivate
{
	class TessellatedQuadGeometry : public Qt3DCore::QGeometry
	{
	public:
		explicit TessellatedQuadGeometry(QVector3D p0, QVector3D p1, QVector3D p2, QVector3D p3, Qt3DCore::QNode* parent = nullptr) :
			Qt3DCore::QGeometry(parent),
			PositionAttribute(new Qt3DCore::QAttribute(this)),
			VertexBuffer(new Qt3DCore::QBuffer(this))
		{
			QByteArray vertexPositionData(sizeof(QVector3D) * 4, Qt::Uninitialized);
			reinterpret_cast<QVector3D&>(vertexPositionData.data()[0 * sizeof(QVector3D)]) = p0;
			reinterpret_cast<QVector3D&>(vertexPositionData.data()[1 * sizeof(QVector3D)]) = p1;
			reinterpret_cast<QVector3D&>(vertexPositionData.data()[2 * sizeof(QVector3D)]) = p2;
			reinterpret_cast<QVector3D&>(vertexPositionData.data()[3 * sizeof(QVector3D)]) = p3;

			VertexBuffer->setData(vertexPositionData);

			PositionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
			PositionAttribute->setVertexBaseType(Qt3DCore::QAttribute::VertexBaseType::Float);
			PositionAttribute->setVertexSize(3);
			PositionAttribute->setCount(4);
			PositionAttribute->setByteStride(3 * sizeof(float));

			addAttribute(PositionAttribute);
		}

	private:
		Qt3DCore::QAttribute* PositionAttribute = nullptr;
		Qt3DCore::QBuffer* VertexBuffer = nullptr;
	};
}

TessellatedQuadRenderer::TessellatedQuadRenderer(QVector3D p0, QVector3D p1, QVector3D p2, QVector3D p3, Qt3DCore::QNode* parent) :
	Qt3DRender::QGeometryRenderer(parent)
{
	setPrimitiveType(Qt3DRender::QGeometryRenderer::Patches);
	setGeometry(new TessellatedQuadRendererPrivate::TessellatedQuadGeometry(p0, p1, p2, p3, this));
	setVerticesPerPatch(4);
}
