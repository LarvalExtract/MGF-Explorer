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
	void SetTerrainCellSize(int32_t cellSize);
	void SetHeightMap(Qt3DRender::QAbstractTexture* texture);
	void SetBaseMap(Qt3DRender::QAbstractTexture* texture);
	void SetDetailBlend(float value);
	void SetDetailMultiplier0(float value);
	void SetDetailMultiplier1(float value);
	void SetDetailMap0(Qt3DRender::QAbstractTexture* texture);
	void SetDetailMap1(Qt3DRender::QAbstractTexture* texture);
	void SetMicroDetailMap0(Qt3DRender::QAbstractTexture* texture);
	void SetMicroDetailMap1(Qt3DRender::QAbstractTexture* texture);
	void SetMicroDetailTiling0(float value);
	void SetMicroDetailTiling1(float value);
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
	Qt3DRender::QParameter* TerrainDetailBlend = nullptr;
	Qt3DRender::QParameter* TerrainDetailMultiplier0 = nullptr;
	Qt3DRender::QParameter* TerrainDetailMultiplier1 = nullptr;
	Qt3DRender::QParameter* TerrainDetailMap0 = nullptr;
	Qt3DRender::QParameter* TerrainDetailMap1 = nullptr;
	Qt3DRender::QParameter* TerrainMicroDetailMap0 = nullptr;
	Qt3DRender::QParameter* TerrainMicroDetailMap1 = nullptr;
	Qt3DRender::QParameter* TerrainMicroDetailTiling0 = nullptr;
	Qt3DRender::QParameter* TerrainMicroDetailTiling1 = nullptr;
	Qt3DRender::QParameter* TerrainMinHeight = nullptr;
	Qt3DRender::QParameter* TerrainMaxHeight = nullptr;
};