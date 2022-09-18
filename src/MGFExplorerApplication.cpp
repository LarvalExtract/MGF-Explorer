#include "MGFExplorerApplication.h"

#include <QMessageBox>
#include <filesystem>

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
	InitialiseOgre();
	RenderWindowContainer = QWidget::createWindowContainer(OgreWindow.get(), &MainWindow);

	MainWindow.show();

	for (const auto& [mgfPath, assetPath] : FileList)
	{
		MainWindow.OpenMGFWorkspace(mgfPath.u8string().c_str());
	}

	return QApplication::exec();
}

OgreWindow* MGFExplorerApplication::GetRenderWindow() const
{
	return OgreWindow.get();
}

QWidget* MGFExplorerApplication::GetRenderWindowContainer() const
{
	return RenderWindowContainer;
}

void MGFExplorerApplication::InitialiseOgre()
{
	OgreRoot = std::make_unique<Ogre::Root>("plugins.cfg", "ogre.cfg", "");
	OgreWindow = std::make_unique<::OgreWindow>();

	const auto& renderers = OgreRoot->getAvailableRenderers();

	if (renderers.empty())
		throw std::runtime_error("No renderers found. Please ensure plugins.cfg is present in the application root.");

	OgreRoot->setRenderSystem(renderers[0]);
	OgreRoot->initialise(false);
	OgreWindow->initialize("OgreWindow");

	OgreRoot->createSceneManager();
}
