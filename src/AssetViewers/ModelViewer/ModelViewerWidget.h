#ifndef MGMODELVIEWERWIDGET_H
#define MGMODELVIEWERWIDGET_H

#include "AssetViewers/IAssetViewerWidget.h"

#include "OgreWindow/ogrewindow.h"

#include <QWidget>

namespace Ui {
    class ModelViewerWidget;
}

namespace ModelViewer {

	class ModelViewerWidget : public QWidget, public IAssetViewerWidget
	{
		Q_OBJECT

	public:
		explicit ModelViewerWidget(QWidget* parent = nullptr);
		~ModelViewerWidget();

		void LoadAsset(MGF::Asset::AssetPtr asset) override;

	private:
		Ui::ModelViewerWidget* ui;

		OgreWindow m_OgreWindow;
		QWidget* m_OgreWindowContainer = nullptr;

		Ogre::SceneManager* m_SceneManager = nullptr;
		Ogre::Camera* m_Camera = nullptr;
		Ogre::Viewport* m_Viewport = nullptr;
		Ogre::SceneNode* m_CameraNode = nullptr;
		Ogre::SceneNode* m_SceneRoot = nullptr;
		float m_CameraSpeed = 1.0f;
		float m_ScrollAngle = 0.0f;

	private:
		QPoint m_CursorLocation;
		bool m_bMouseHeld = false;
		float m_CameraPitch = 0.0f;
		float m_CameraYaw = 0.0f;

		Ogre::Vector3 m_Forward = Ogre::Vector3::NEGATIVE_UNIT_Z;
		Ogre::Vector3 m_Right = Ogre::Vector3::NEGATIVE_UNIT_X;
		Ogre::Vector3 m_Up = Ogre::Vector3::NEGATIVE_UNIT_Y;

		bool m_bIsWPressed = false;
		bool m_bIsAPressed = false;
		bool m_bIsSPressed = false;
		bool m_bIsDPressed = false;
		bool m_bIsSpacePressed = false;
		bool m_bIsCtrlPressed = false;

	private:
		void InitialiseScene();
		void UpdateFrame(QTimerEvent* timerEvt);
		void ResetCamera(const Ogre::AxisAlignedBox& modelAABB);
		void UpdateCamera();
		void UpdateCameraAxes(float yaw, float pitch);
		void UpdateKeyState(int key, bool state);

	public:
		bool eventFilter(QObject* watched, QEvent* event) override;
	};

}

#endif // MGMODELVIEWERWIDGET_H
