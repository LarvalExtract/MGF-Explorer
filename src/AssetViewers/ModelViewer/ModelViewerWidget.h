#pragma once

#include "AssetViewers/IAssetViewerWidget.h"
#include "AssetViewers/3DSceneWidget.h"

namespace Qt3DCore
{
	class QEntity;
	class QTransform;
}

namespace Qt3DExtras 
{
	class QAbstractCameraController;
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

		void LoadAsset(MGF::Asset::AssetPtr asset) override;

	private:
		Ui::ModelViewerWidget* ui;
		TestWidget* SceneWidget = nullptr;

	private slots:
		void on_lightPositionXInput_changed(int value);
		void on_lightPositionYInput_changed(int value);
		void on_lightPositionZInput_changed(int value);
		void on_enableSceneLightsCheckBox_stateChanged(int state);
	};

}
