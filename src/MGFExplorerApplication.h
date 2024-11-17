#pragma once

#include "MainWindow/MainWindow.h"
#include "MGF/AssetManager.h"
#include "MGF/Assets/Factories/MeshLibrary.h"

#include <QApplication>

#include <filesystem>
#include <vector>

class Scene3dWidget;

namespace Qt3DCore
{
	class QEntity;
}

namespace Qt3DRender
{
	class QCamera;
}

namespace Qt3DExtras
{
	class QTextureMaterial;
}

#undef qApp
#define qApp (static_cast<MGFExplorerApplication*>(QCoreApplication::instance()))

class MGFExplorerApplication final : public QApplication
{
public:
	explicit MGFExplorerApplication(int argc, char* argv[], int flags = ApplicationFlags);
	~MGFExplorerApplication() override = default;

	MGFExplorerApplication() = delete;
	MGFExplorerApplication(const MGFExplorerApplication&) = delete;
	MGFExplorerApplication(MGFExplorerApplication&&) = delete;
	MGFExplorerApplication& operator=(const MGFExplorerApplication&) = delete;
	MGFExplorerApplication& operator=(MGFExplorerApplication&&) = delete;

	int exec();

	Scene3dWidget* SceneWidget = nullptr;

	MGF::AssetManager AssetManager;
	MA::MeshLibrary mMeshLibrary;

private:
	MainWindow MainWindow;

	std::vector<std::pair<std::filesystem::path, std::filesystem::path>> FileList;
};