#pragma once

#include "MainWindow/MainWindow.h"
#include "MGF/AssetManager.h"
#include "MGF/Assets/Factories/MeshLibrary.h"

#include "MGF/Archive.h"

#include <QApplication>
#include <QSettings>

#include <filesystem>
#include <vector>

class Scene3dWidget;

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

	std::filesystem::path GetMgfFolderFromAppSettings() const;
	void SetMgfFolderAppSetting(const std::filesystem::path& MgfFolder);

	MGF::Archive* GetMgfArchive(const std::filesystem::path& MgfArchivePath);

private:
	QSettings AppSettings;
	MainWindow MainWindow;

	std::vector<std::pair<std::filesystem::path, std::filesystem::path>> FileList;
	std::unordered_map<std::filesystem::path, MGF::Archive> MgfArchiveMap;
};