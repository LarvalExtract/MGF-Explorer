#pragma once

#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>

#include "Qt3DForwardDeclarations.h"

class QBoxLayout;
class QScrollArea;

class Scene3dWidget : public QWidget
{
	Q_OBJECT

public:
	explicit Scene3dWidget(QWidget* parent = nullptr);

	QWidget* GetContainerWidget() const;

	Qt3DRender::QCamera* GetCamera() const;

	void SetEntity(Qt3DCore::QEntity* root);
	void SetCamera(Qt3DRender::QCamera* camera);

	Qt3DRender::QLayer* GetTransparentLayer() const;
	Qt3DRender::QLayer* GetOpaqueLayer() const;

private:
	QWidget* Container = nullptr;
	Qt3DExtras::Qt3DWindow* RenderWindow = nullptr;

	Qt3DCore::QEntity* SceneRoot = nullptr;

	Qt3DRender::QRenderSurfaceSelector* RenderSurfaceSelector = nullptr;
	Qt3DRender::QClearBuffers* ClearBuffers = nullptr;
	Qt3DRender::QViewport* Viewport = nullptr;
	Qt3DRender::QCameraSelector* CameraSelector = nullptr;
	Qt3DRender::QCamera* Camera = nullptr;

	Qt3DRender::QTechniqueFilter* TechniqueFilter = nullptr;
	Qt3DRender::QLayerFilter* OpaqueLayerFilter = nullptr;
	Qt3DRender::QLayerFilter* TransparentLayerFilter = nullptr;
	Qt3DRender::QLayer* OpaqueLayer = nullptr;
	Qt3DRender::QLayer* TransparentLayer = nullptr;

	Qt3DRender::QSortPolicy* OpaqueSortPolicy = nullptr;
	Qt3DRender::QSortPolicy* TransparentSortPolicy = nullptr;
};

class TestWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TestWidget(QBoxLayout* frameLayout, QWidget* parent = nullptr);
	explicit TestWidget(QScrollArea* scrollArea, QWidget* parent = nullptr);

	Qt3DRender::QCamera* GetCamera() const;

	void SetModelEntity(Qt3DCore::QEntity* root);
	void SetCamera(Qt3DRender::QCamera* camera);

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	Scene3dWidget* RenderWindowContainer = nullptr;
	Qt3DCore::QEntity* RootEntity = nullptr;
	Qt3DExtras::QAbstractCameraController* CameraControllerEntity = nullptr;
	Qt3DCore::QEntity* ModelEntity = nullptr;

private:
	int WindowTimerHandle = 0;
	QBoxLayout* FrameLayout = nullptr;
	QScrollArea* ScrollArea = nullptr;
};