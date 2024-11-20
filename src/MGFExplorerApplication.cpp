#include "MGFExplorerApplication.h"
#include "AssetViewers/3DSceneWidget.h"

#include <QMessageBox>

MGFExplorerApplication::MGFExplorerApplication(int argc, char* argv[], int flags)
	: QApplication(argc, argv, flags)
{
	setOrganizationName("LarvalExtract");
	setApplicationName("MGF Explorer");

	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path p(argv[i]);

		if (!p.is_absolute())
		{
			p = GetMgfFolderFromAppSettings() / p;
		}

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

	SceneWidget = new Scene3dWidget;

	MainWindow.show();

	for (const auto& [mgfPath, assetPath] : FileList)
	{
		MainWindow.OpenMGFWorkspace(mgfPath, &assetPath);
	}

	return QApplication::exec();
}

std::filesystem::path MGFExplorerApplication::GetMgfFolderFromAppSettings() const
{
	QSettings settings;
	const QString defaultMgfFolderKey = "SavedMgfFolder";

	std::string s = settings.value(defaultMgfFolderKey).toString().toStdString();
	return s;
}

void MGFExplorerApplication::SetMgfFolderAppSetting(const std::filesystem::path& MgfFolder)
{
	QSettings settings;
	const QString defaultMgfFolderKey = "SavedMgfFolder";

	settings.setValue(defaultMgfFolderKey, QString(MgfFolder.u8string().c_str()));
}
