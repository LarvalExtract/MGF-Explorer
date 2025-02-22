#include "MATerrain.h"

#include "Render/TessellatedQuad.h"

#include <QEntity>
#include <Qt3DCore/QTransform>
#include <QShaderProgram>
#include <QParameter>
#include <QMaterial>
#include <QTechnique>
#include <QGraphicsApiFilter>
#include <QRenderPass>
#include <QUrl>
#include <QDir>
#include <QParameter>
#include <QBlendEquation>
#include <QBlendEquationArguments>

MATerrain::MATerrain(Qt3DCore::QNode* parent) :
	QEntity(parent),
	TerrainCellSize(new Qt3DRender::QParameter("TerrainCellSize", 16.0f, this)),
	TerrainWorldScale(new Qt3DRender::QParameter("TerrainWorldScale", 8.0f, this)),
	TerrainFullSize(new Qt3DRender::QParameter("TerrainFullSize", 2048.0f, this)),
	TerrainMinHeight(new Qt3DRender::QParameter("TerrainMinHeight", 0.0f, this)),
	TerrainMaxHeight(new Qt3DRender::QParameter("TerrainMaxHeight", 100.0f, this)),
	TerrainHeightMap(new Qt3DRender::QParameter("TerrainHeightMap", nullptr, this)),
	TerrainBaseMap(new Qt3DRender::QParameter("TerrainBaseMap", nullptr, this))
{
	Qt3DRender::QMaterial* material = new Qt3DRender::QMaterial(this);
	{
		Qt3DRender::QEffect* effect = new Qt3DRender::QEffect(material);
		{
			Qt3DRender::QTechnique* technique = new Qt3DRender::QTechnique(effect);
			technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
			technique->graphicsApiFilter()->setMajorVersion(4);
			technique->graphicsApiFilter()->setMinorVersion(0);
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

	addComponent(material);
}

void MATerrain::SetTerrainPosition(QVector3D position)
{
	Position = position;
}

void MATerrain::SetTerrainWorldScale(float worldScale)
{
	TerrainWorldScale->setValue(worldScale);
}

void MATerrain::SetTerrainCellSize(float cellSize)
{
	TerrainCellSize->setValue(cellSize);
}

void MATerrain::SetHeightMap(Qt3DRender::QAbstractTexture* texture)
{
	TerrainHeightMap->setValue(QVariant::fromValue(texture));
}

void MATerrain::SetBaseMap(Qt3DRender::QAbstractTexture* texture)
{
	TerrainBaseMap->setValue(QVariant::fromValue(texture));
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
			const QVector3D p0 = Position + QVector3D(0.0f,		0.0f,		0.0f); // p0----p1
			const QVector3D p1 = Position + QVector3D(quadSize, 0.0f,		0.0f); // |		|
			const QVector3D p2 = Position + QVector3D(quadSize, quadSize,	0.0f); // |		|
			const QVector3D p3 = Position + QVector3D(0.0f,		quadSize,	0.0f); // p3----p2

			addComponent(new TessellatedQuadRenderer(p0, p1, p2, p3, this));
		}
	}
}
