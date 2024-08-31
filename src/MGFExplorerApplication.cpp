#include "MGFExplorerApplication.h"
#include "AssetViewers/TextureViewer/TextureViewerWidget.h"
#include "AssetViewers/ModelViewer/ModelViewerWidget.h"

#include <QMessageBox>

#include <filesystem>

#include <QDebugOverlay>
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>
#include <QPlaneMesh>
#include <QTextureMaterial>
#include <QTransform>
#include <QPointLight>
#include <QSortPolicy>
#include <QFilterKey>
#include <QTechniqueFilter>

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
	RenderWindowContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	Qt3DRender::QRenderSurfaceSelector* renderSurfaceSelector = new Qt3DRender::QRenderSurfaceSelector;
	renderSurfaceSelector->setSurface(RenderWindow);

	Qt3DRender::QClearBuffers* clearBuffers = new Qt3DRender::QClearBuffers(renderSurfaceSelector);
	clearBuffers->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);
	clearBuffers->setClearColor(QColorConstants::DarkCyan);
		
	Qt3DRender::QViewport* viewport = new Qt3DRender::QViewport(renderSurfaceSelector);
			
	Qt3DRender::QCameraSelector* cameraSelector = new Qt3DRender::QCameraSelector(viewport);
			
	RenderWindow->setActiveFrameGraph(renderSurfaceSelector);

	ModelViewer::ModelViewerWidget::InitialiseScene(RenderWindow, cameraSelector);

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
