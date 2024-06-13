#pragma once

#include "AssetViewers/IAssetViewerWidget.h"

namespace Qt3DCore
{
	class QEntity;
	class QTransform;
}

namespace Qt3DExtras 
{
	class QAbstractCameraController;
	class QCuboidMesh;
	class Qt3DWindow;
}

namespace Qt3DRender
{
	class QCamera;
	class QCameraSelector;
	class QPointLight;
	class QDirectionalLight;
	class QLayer;
}

namespace Ui {
    class ModelViewerWidget;
}

namespace ModelViewer {

	class ModelViewerWidget : public IAssetViewerWidget
	{
		Q_OBJECT

	public:
		explicit ModelViewerWidget(QWidget* parent = nullptr);
		~ModelViewerWidget();

		static bool InitialiseScene(Qt3DExtras::Qt3DWindow* renderWindow, Qt3DRender::QCameraSelector* cameraSelector);

		void LoadAsset(MGF::Asset::AssetPtr asset) override;

	private:
		Ui::ModelViewerWidget* ui;
		int m_WindowTimerId = 0;

		static Qt3DExtras::Qt3DWindow* RenderWindowPtr;
		static Qt3DRender::QCamera* Camera;

	public:
		static Qt3DCore::QEntity* SceneRoot;
		static Qt3DCore::QEntity* SceneLightEntity;
		static Qt3DRender::QPointLight* Light;
		static Qt3DCore::QTransform* LightTransform;
		static Qt3DRender::QCamera* ModelViewerCamera;
		static Qt3DRender::QCameraSelector* CameraSelector;
		static Qt3DExtras::QAbstractCameraController* CameraController;
		static Qt3DRender::QLayer* OpaqueLayer;
		static Qt3DRender::QLayer* TransparentLayer;

		Qt3DCore::QEntity* CurrentModelEntity = nullptr;

	private slots:
		void on_lightPositionXInput_changed(int value);
		void on_lightPositionYInput_changed(int value);
		void on_lightPositionZInput_changed(int value);

	protected:
		void showEvent(QShowEvent* event) override;
		void hideEvent(QHideEvent* event) override;
	};

}
