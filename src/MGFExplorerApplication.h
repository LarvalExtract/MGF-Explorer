#pragma once

#include "MainWindow/MainWindow.h"
#include "MGF/AssetManager.h"

#include "OgreWindow/ogrewindow.h"

#include <QApplication>

#include <memory>
#include <filesystem>
#include <vector>

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

	OgreWindow* GetRenderWindow() const;
	QWidget* GetRenderWindowContainer() const;

	MGF::AssetManager AssetManager;
	Ogre::SceneManager* SceneManager;

private:
	MainWindow MainWindow;
	QWidget* RenderWindowContainer = nullptr;
	std::unique_ptr<Ogre::Root> OgreRoot;
	std::unique_ptr<OgreWindow> OgreWindow;

	std::vector<std::pair<std::filesystem::path, std::filesystem::path>> FileList;

	void InitialiseOgre();
};