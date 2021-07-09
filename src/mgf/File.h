#pragma once

#include "EFileTypes.h"
#include "VersionConstants.h"

#include <QDateTime>
#include <QHash>

#include <vector>
#include <filesystem>
#include <unordered_map>

namespace MGF {

    class Archive;

    class File
    {
    public:
        File(
            File* parent,
            const char* name,
            uint64_t id,
            uint32_t index,
            uint32_t offset,
            uint32_t length,
            int32_t  timestamp,
            bool     isFile,
            Archive& mgfFile
        );

        const File*     Parent;
        const std::filesystem::path FilePath;
        const QString   Name;
        const uint64_t  GUID;
        const uint32_t  Index;
        const EFileType FileType;
        const uint32_t  FileOffset;
        const uint32_t  FileLength;
        const QDateTime FileDate;
        const bool      IsFile;
        Archive&        MGFArchive;
        const Version   ArchiveVersion;
        const int       TreeViewRow;
    	
        File* GetNthChild(int index) const;
        File* GetNamedChild(const QString& name) const;
        File* GetNamedSibling(const QString& name) const;
        void AddChildItem(File* item);
        size_t GetChildCount() const;
        const auto& GetChildren() const { return Children; }

        // takes a relative file path and traverses the MGF file system to find the desired item
        const File* FindRelativeItem(const std::filesystem::path& relativePath) const;

        void Read(char* buffer, uint32_t offset = 0, uint32_t length = INT_MAX) const;

        template<typename T>
        T Read(size_t offset) const
        {
            T result;
            this->Read(reinterpret_cast<char*>(&result), offset, sizeof(result));
            return result;
        }

    private:
        QHash<QString, File*> Children;
        std::vector<File*> ChildrenArray;

        static const std::unordered_map<std::string, EFileType>  MapExtensionFileType;
    };

}
