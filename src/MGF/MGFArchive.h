#pragma once

#include "MGFFile.h"
#include "Assets/MGFAsset.h"

#include <fstream>
#include <QAbstractItemModel>

enum class MGFArchiveVersion : char
{
    MechAssault = 2,
    MechAssault2LW = 4
};

inline const char* ToString(MGFArchiveVersion version)
{
    switch (version)
    {
    case MGFArchiveVersion::MechAssault: return "MechAssault";
    case MGFArchiveVersion::MechAssault2LW: return "MechAssault 2: Lone Wolf";
    }
}

class MGFArchive : protected std::ifstream, public QAbstractItemModel
{
    friend class MGFFile;
    friend class MGFFileDeserializer;

public:
    MGFArchive(const std::filesystem::path& mgfFilePath);
	
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	
    const std::filesystem::path Path;
    MGFArchiveVersion GetVersion() const { return ArchiveVersion; }
    size_t GetFileSize() const { return Size; }
    uint32_t GetFileCount() const { return FileCount; }
    const MGFFile* Root() const { return &Files[0]; }

    template<typename AssetType>
    std::shared_ptr<AssetType> LoadAsset(const std::filesystem::path& path)
    {
        static_assert(std::is_convertible<AssetType*, MGFAsset*>::value, "AssetType must derive from MGFAsset");

        if (const MGFFile* file = Root()->FindRelativeItem(path))
        {
            return std::static_pointer_cast<AssetType>(LoadAsset(*file));
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<MGFAsset> LoadAsset(const MGFFile& mgfFile);

private:  	
    MGFArchiveVersion ArchiveVersion = MGFArchiveVersion::MechAssault;
    size_t Size = 0;
	uint32_t FileCount = 0;
    std::vector<MGFFile> Files;
};

