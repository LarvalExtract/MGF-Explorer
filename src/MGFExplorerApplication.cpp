#include "MGFExplorerApplication.h"

#include <QMessageBox>

#include <filesystem>

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QPlaneMesh>
#include <QTextureMaterial>
#include <QTransform>
#include <QPointLight>

MGFExplorerApplication::MGFExplorerApplication(int argc, char* argv[], int flags)
	: QApplication(argc, argv, flags)
{
	setOrganizationName("LarvalExtract");
	setApplicationName("MGF Explorer");

	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path p(argv[i]);
		for (auto iter = p.begin(); iter != p.end(); ++iter)
		{
			if (iter->extension() == ".mgf")
			{
				if (const auto mgfPath = std::accumulate(p.begin(), ++iter, std::filesystem::path{}, std::divides{}); exists(mgfPath))
				{
					const auto assetPath = std::accumulate(iter, p.end(), std::filesystem::path{}, std::divides{});
					FileList.emplace_back(std::make_pair(mgfPath, assetPath));
					break;
				}
			}
		}
	}
}

int MGFExplorerApplication::exec()
{
	qputenv("QT3D_RENDERER", "opengl");

	RenderWindow = new Qt3DExtras::Qt3DWindow(nullptr, Qt3DRender::API::OpenGL);
	RenderWindowContainer = QWidget::createWindowContainer(RenderWindow, &MainWindow);

	SetupTextureViewerScene();
	SetupModelViewerScene();

	MainWindow.show();

	for (const auto& [mgfPath, assetPath] : FileList)
	{
		MainWindow.OpenMGFWorkspace(mgfPath);
	}

	return QApplication::exec();
}

Qt3DExtras::Qt3DWindow* MGFExplorerApplication::GetRenderWindow() const
{
	return RenderWindow;
}

QWidget* MGFExplorerApplication::GetRenderWindowContainer() const
{
	return RenderWindowContainer;
}

MGFExplorerApplication::TextureViewerData MGFExplorerApplication::GetTextureViewerData()
{
	RenderWindow->setRootEntity(mTextureViewerRootEntity);

	RenderWindow->camera()->lens()->setOrthographicProjection(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);
	RenderWindow->camera()->setPosition(QVector3D(0.0f, 0.0f, 1.0f));
	RenderWindow->camera()->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
	RenderWindow->camera()->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

	return TextureViewerData{
		RenderWindow->camera(),
		mTextureViewerMaterial
	};
}

MGFExplorerApplication::ModelViewerData MGFExplorerApplication::GetModelViewerData()
{
	if (mLastEntity)
	{
		mLastEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	RenderWindow->setRootEntity(mModelViewerRootEntity);

	auto camera = RenderWindow->camera();
	camera->lens()->setPerspectiveProjection(55.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	camera->setPosition(QVector3D(0.0f, 0.0f, 20.0f));
	camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
	camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

	return ModelViewerData{
		RenderWindow,
		mModelViewerRootEntity
	};
}

void MGFExplorerApplication::SetupTextureViewerScene()
{
	mTextureViewerRootEntity = new Qt3DCore::QEntity();

	mTextureViewerMaterial = new Qt3DExtras::QTextureMaterial();
	auto plane = new Qt3DExtras::QPlaneMesh();

	auto trnsfm = new Qt3DCore::QTransform();
	trnsfm->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
	trnsfm->setRotation(QQuaternion::fromEulerAngles(QVector3D(90.0f, 0.0f, 0.0f)));
	trnsfm->setScale(1.0f);
	
	mTextureViewerRootEntity->addComponent(plane);
	mTextureViewerRootEntity->addComponent(mTextureViewerMaterial);
	mTextureViewerRootEntity->addComponent(trnsfm);
}

void MGFExplorerApplication::SetupModelViewerScene()
{
	mModelViewerRootEntity = new Qt3DCore::QEntity();
	auto trnsfm = new Qt3DCore::QTransform();
	trnsfm->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
	trnsfm->setRotation(QQuaternion::fromEulerAngles(QVector3D(0.0f, 0.0f, 0.0f)));
	trnsfm->setScale(1.0f);

	auto cameraController = new Qt3DExtras::QFirstPersonCameraController(mModelViewerRootEntity);
	cameraController->setCamera(RenderWindow->camera());

	auto light = new Qt3DRender::QPointLight;
	light->setConstantAttenuation(1.0f);
	light->setColor(QColor::fromRgbF(1.0f, 1.0f, 1.0f));
	light->setIntensity(100.0f);

	mModelViewerRootEntity->addComponent(light);
}
