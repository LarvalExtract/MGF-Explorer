#pragma once

#include "MainWindow/MainWindow.h"
#include "MGF/AssetManager.h"
#include "MGF/Assets/Factories/TextureLibrary.h"

#include <QApplication>
#include <Qt3DWindow>

#include <memory>
#include <filesystem>
#include <vector>

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

	Qt3DExtras::Qt3DWindow* GetRenderWindow() const;
	QWidget* GetRenderWindowContainer() const;

	MGF::AssetManager AssetManager;
	MA::TextureLibrary mTextureLibrary;

	struct TextureViewerData
	{
		Qt3DRender::QCamera* pCamera;
		Qt3DExtras::QTextureMaterial* pMaterial;
	} GetTextureViewerData();

private:
	MainWindow MainWindow;
	Qt3DExtras::Qt3DWindow* RenderWindow = nullptr;
	QWidget* RenderWindowContainer = nullptr;

	std::vector<std::pair<std::filesystem::path, std::filesystem::path>> FileList;

	void SetupTextureViewerScene();
	void SetupModelViewerScene();

	Qt3DCore::QEntity* mTextureViewerRootEntity = nullptr;
	Qt3DExtras::QTextureMaterial* mTextureViewerMaterial = nullptr;
};