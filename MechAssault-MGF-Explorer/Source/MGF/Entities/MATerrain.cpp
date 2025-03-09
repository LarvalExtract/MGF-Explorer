#include "MATerrain.h"

#include "Render/TessellatedQuad.h"

#include <QEntity>
#include <Qt3DCore/QTransform>
#include <QParameter>
#include <QMaterial>
#include <QTechnique>
#include <QGraphicsApiFilter>
#include <QRenderPass>
#include <QUrl>
#include <QDir>
#include <QBlendEquation>
#include <QBlendEquationArguments>

MATerrain::MATerrain(Qt3DCore::QNode* parent) :
	QEntity(parent),
	TerrainCellSize(new Qt3DRender::QParameter("TerrainCellSize", 16.0f)),
	TerrainWorldScale(new Qt3DRender::QParameter("TerrainWorldScale", 8.0f)),
	TerrainFullSize(new Qt3DRender::QParameter("TerrainFullSize", 2048.0f)),
	TerrainMinHeight(new Qt3DRender::QParameter("TerrainMinHeight", 0.0f)),
	TerrainMaxHeight(new Qt3DRender::QParameter("TerrainMaxHeight", 100.0f)),
	TerrainHeightMap(new Qt3DRender::QParameter("TerrainHeightMap", nullptr)),
	TerrainDetailBlend(new Qt3DRender::QParameter("TerrainDetailBlend", 0.0f)),
	TerrainDetailMap0(new Qt3DRender::QParameter("TerrainDetailMap0", nullptr)),
	TerrainDetailMap1(new Qt3DRender::QParameter("TerrainDetailMap1", nullptr)),
	TerrainDetailMultiplier0(new Qt3DRender::QParameter("TerrainDetailMultiplier0", 0.0f)),
	TerrainDetailMultiplier1(new Qt3DRender::QParameter("TerrainDetailMultiplier1", 0.0f)),
	TerrainMicroDetailMap0(new Qt3DRender::QParameter("TerrainMicroDetailMap0", nullptr)),
	TerrainMicroDetailMap1(new Qt3DRender::QParameter("TerrainMicroDetailMap1", nullptr)),
	TerrainMicroDetailTiling0(new Qt3DRender::QParameter("TerrainMicroDetailTiling0", 0.0f)),
	TerrainMicroDetailTiling1(new Qt3DRender::QParameter("TerrainMicroDetailTiling1", 0.0f)),
	TerrainBaseMap(new Qt3DRender::QParameter("TerrainBaseMap", nullptr))
{
	
}

void MATerrain::SetTerrainPosition(QVector3D position)
{
	Position = position;
}

void MATerrain::SetTerrainWorldScale(float worldScale)
{
	TerrainWorldScale->setValue(worldScale);
}

void MATerrain::SetTerrainCellSize(int32_t cellSize)
{
	TerrainCellSize->setValue((float)cellSize);
}

void MATerrain::SetHeightMap(Qt3DRender::QAbstractTexture* texture)
{
	TerrainHeightMap->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetBaseMap(Qt3DRender::QAbstractTexture* texture)
{
	TerrainBaseMap->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetDetailBlend(float value)
{
	TerrainDetailBlend->setValue(value);
}

void MATerrain::SetDetailMultiplier0(float value)
{
	TerrainDetailMultiplier0->setValue(value);
}

void MATerrain::SetDetailMultiplier1(float value)
{
	TerrainDetailMultiplier1->setValue(value);
}

void MATerrain::SetDetailMap0(Qt3DRender::QAbstractTexture* texture)
{
	TerrainDetailMap0->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetDetailMap1(Qt3DRender::QAbstractTexture* texture)
{
	TerrainDetailMap1->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetMicroDetailMap0(Qt3DRender::QAbstractTexture* texture)
{
	TerrainMicroDetailMap0->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetMicroDetailMap1(Qt3DRender::QAbstractTexture* texture)
{
	TerrainMicroDetailMap1->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetMicroDetailTiling0(float value)
{
	TerrainMicroDetailTiling0->setValue(value);
}

void MATerrain::SetMicroDetailTiling1(float value)
{
	TerrainMicroDetailTiling1->setValue(value);
}

void MATerrain::SetMinHeight(float minHeight)
{
	TerrainMinHeight->setValue(minHeight);
}

void MATerrain::SetMaxHeight(float maxHeight)
{
	TerrainMaxHeight->setValue(maxHeight);
}

void MATerrain::BuildTerrain()
{
	const float cellSize = TerrainCellSize->value().toFloat();
	const float size = TerrainWorldScale->value().toFloat() * cellSize * cellSize;
	const int cells = 1; // number of draw calls is cells^2
	const float quadSize = size / static_cast<float>(cells);

	TerrainFullSize->setValue(size);

	for (int x = 0; x < cells; ++x)
	{
		for (int y = 0; y < cells; ++y)
		{
			const QVector3D p0 = Position + QVector3D(0.0f,		0.0f,	0.0f);
			const QVector3D p1 = Position + QVector3D(quadSize, 0.0f,	0.0f);
			const QVector3D p2 = Position + QVector3D(quadSize, 0.0f,	quadSize); 
			const QVector3D p3 = Position + QVector3D(0.0f,		0.0f,	quadSize); 

			addComponent(new TessellatedQuadRenderer(p0, p1, p2, p3, this));
		}
	}

	Qt3DRender::QMaterial* material = new Qt3DRender::QMaterial(this);
	{
		Qt3DRender::QEffect* effect = new Qt3DRender::QEffect(material);
		{
			Qt3DRender::QTechnique* technique = new Qt3DRender::QTechnique(effect);
			technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
			technique->graphicsApiFilter()->setMajorVersion(4);
			technique->graphicsApiFilter()->setMinorVersion(0);
			technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::OpenGLProfile::CoreProfile);
			{
				Qt3DRender::QRenderPass* baseRenderPass = new Qt3DRender::QRenderPass(technique);
				{
					Qt3DRender::QBlendEquation* blendEquation = new Qt3DRender::QBlendEquation(baseRenderPass);
					blendEquation->setBlendFunction(Qt3DRender::QBlendEquation::Add);

					Qt3DRender::QBlendEquationArguments* blendState = new Qt3DRender::QBlendEquationArguments(baseRenderPass);
					blendState->setSourceRgb(Qt3DRender::QBlendEquationArguments::One);
					blendState->setDestinationRgb(Qt3DRender::QBlendEquationArguments::Zero);

					Qt3DRender::QShaderProgram* terrainShader = new Qt3DRender::QShaderProgram(baseRenderPass);
					terrainShader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/VertexPassthrough.vert")));
					terrainShader->setTessellationControlShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Quads.tcs")));
					terrainShader->setTessellationEvaluationShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/Quads.tes")));
					terrainShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(QDir::currentPath() + "/Shaders/MATerrain.frag")));

					baseRenderPass->addRenderState(blendEquation);
					baseRenderPass->addRenderState(blendState);
					baseRenderPass->setShaderProgram(terrainShader);

					technique->addRenderPass(baseRenderPass);
				}
			}

			effect->addTechnique(technique);
		}

		material->setEffect(effect);
	}

	material->addParameter(TerrainCellSize);
	material->addParameter(TerrainWorldScale);
	material->addParameter(TerrainFullSize);
	material->addParameter(TerrainMinHeight);
	material->addParameter(TerrainMaxHeight);
	material->addParameter(TerrainHeightMap);
	material->addParameter(TerrainBaseMap);
	material->addParameter(TerrainDetailBlend);
	material->addParameter(TerrainDetailMultiplier0);
	material->addParameter(TerrainDetailMultiplier1);
	material->addParameter(TerrainDetailMap0);
	material->addParameter(TerrainDetailMap1);
	material->addParameter(TerrainMicroDetailMap0);
	material->addParameter(TerrainMicroDetailMap1);
	material->addParameter(TerrainMicroDetailTiling0);
	material->addParameter(TerrainMicroDetailTiling1);

	addComponent(material);
}
