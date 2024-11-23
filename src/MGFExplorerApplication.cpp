#include "MGFExplorerApplication.h"
#include "Widgets/3DSceneWidget.h"

#include "MGF/Assets/MGFAsset.h"

#include <QMessageBox>

MGFExplorerApplication::MGFExplorerApplication(int argc, char* argv[], int flags)
	: QApplication(argc, argv, flags)
	, AppSettings("LarvalExtract", "MGF Explorer")
{
	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path p(argv[i]);

		if (p.is_relative())
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
		if (std::shared_ptr<MGFArchive> archive = GetMgfArchive(mgfPath))
		{
			MainWindow.OpenMGFWorkspace(archive, &assetPath);
		}
	}

	return QApplication::exec();
}

std::filesystem::path MGFExplorerApplication::GetMgfFolderFromAppSettings() const
{
	const QString defaultMgfFolderKey = "SavedMgfFolder";

	return AppSettings.value(defaultMgfFolderKey).toString().toStdString();
}

void MGFExplorerApplication::SetMgfFolderAppSetting(const std::filesystem::path& MgfFolder)
{
	const QString defaultMgfFolderKey = "SavedMgfFolder";

	AppSettings.setValue(defaultMgfFolderKey, QString(MgfFolder.u8string().c_str()));
}

std::shared_ptr<MGFArchive> MGFExplorerApplication::GetMgfArchive(const std::filesystem::path& MgfArchivePath)
{
	const std::filesystem::path key = MgfArchivePath.is_absolute() 
		? MgfArchivePath 
		: GetMgfFolderFromAppSettings() / MgfArchivePath.filename();

	if (std::filesystem::exists(key))
	{
		if (!MgfArchiveMap.contains(key) || MgfArchiveMap.at(key).expired())
		{
			std::shared_ptr<MGFArchive> newArchive = std::make_shared<MGFArchive>(key);
			MgfArchiveMap[key] = newArchive;
			return newArchive;
		}

		return MgfArchiveMap.at(key).lock();
	}

	return nullptr;
}

std::shared_ptr<MGFAsset> MGFExplorerApplication::GetAsset(const MGFFile& mgfFile)
{
	const uint32_t key = mgfFile.FilepathHash;

	if (!AssetMap.contains(key) || AssetMap.at(key).expired())
	{
		std::shared_ptr<MGFAsset> newAsset = MGFAsset::Create(mgfFile);
		AssetMap[key] = newAsset;
		return newAsset;
	}

	return AssetMap.at(key).lock();
}
