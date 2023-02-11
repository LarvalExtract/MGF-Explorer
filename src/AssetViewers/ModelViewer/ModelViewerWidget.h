#pragma once

#include "AssetViewers/IAssetViewerWidget.h"

namespace Qt3DCore
{
	class QEntity;
}

namespace Qt3DExtras 
{
	class QFirstPersonCameraController;
	class QCuboidMesh;
}

namespace Qt3DRender
{
	class QCamera;
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
		
		float m_CameraSpeed = 1.0f;
		float m_ScrollAngle = 0.0f;

		int m_WindowTimerId = 0;

	protected:
		void showEvent(QShowEvent* event) override;
		void hideEvent(QHideEvent* event) override;
		bool eventFilter(QObject* object, QEvent* event) override;
	};

}
