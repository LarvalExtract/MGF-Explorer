#pragma once

#include <Qt3DRender/QGeometryRenderer>

class TessellatedQuadRenderer : public Qt3DRender::QGeometryRenderer
{
public:
	explicit TessellatedQuadRenderer(QVector3D p0, QVector3D p1, QVector3D p2, QVector3D p3, Qt3DCore::QNode* parent = nullptr);
};