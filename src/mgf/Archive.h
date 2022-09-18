#pragma once

#include "File.h"
#include "VersionConstants.h"

#include <fstream>
#include <QAbstractItemModel>

struct MGF_HEADER;
struct MGF_FILE_RECORD_MA1;
struct MGF_FILE_RECORD_MA2;
struct MGF_DIRECTORY;

namespace MGF {

    class Archive : protected std::ifstream, public QAbstractItemModel
    {
        friend class File;
        friend class Deserializer;

    public:
        Archive(const std::filesystem::path& mgfFilePath);
    	
        QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex& child) const override;
        // QModelIndex sibling(int row, int column, const QModelIndex& idx) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    	
        const std::filesystem::path Path;
        Version GetVersion() const { return ArchiveVersion; }
        size_t GetFileSize() const { return Size; }
        uint32_t GetFileCount() const { return FileCount; }
        auto Root() const { return &Files[0]; }

    private:  	
        Version ArchiveVersion = Version::MechAssault;
        size_t Size = 0;
		uint32_t FileCount = 0;
        std::vector<File> Files;
    };
}
