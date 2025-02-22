#pragma once

#include "Qt3DForwardDeclarations.h"

#include <Qt3DCore/QEntity>
#include <QVector3D>

class MATerrain : public Qt3DCore::QEntity
{
public:
	explicit MATerrain(Qt3DCore::QNode* parent = nullptr);

	void SetTerrainPosition(QVector3D position);
	void SetTerrainWorldScale(float worldScale);
	void SetTerrainCellSize(float cellSize);
	void SetHeightMap(Qt3DRender::QAbstractTexture* texture);
	void SetBaseMap(Qt3DRender::QAbstractTexture* texture);
	void SetMinHeight(float minHeight);
	void SetMaxHeight(float maxHeight);
	void BuildTerrain();

private:
	QVector3D Position;

	Qt3DRender::QParameter* TerrainCellSize = nullptr;
	Qt3DRender::QParameter* TerrainWorldScale = nullptr;
	Qt3DRender::QParameter* TerrainFullSize = nullptr;
	Qt3DRender::QParameter* TerrainHeightMap = nullptr;
	Qt3DRender::QParameter* TerrainBaseMap = nullptr;
	Qt3DRender::QParameter* TerrainMinHeight = nullptr;
	Qt3DRender::QParameter* TerrainMaxHeight = nullptr;
};