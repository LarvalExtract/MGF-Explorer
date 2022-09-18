#pragma once

#include "EFileTypes.h"
#include "VersionConstants.h"

#include <QDateTime>

#include <filesystem>
#include <vector>

namespace MGF {

    class Archive;

    class File
    {
        friend class Archive;

    private:
        File* m_Parent = nullptr;
        std::vector<const File*> m_Children;
        int m_Row = 0;
        std::filesystem::path m_FilePath;
    
    public:
        File(
            Archive& mgfFile,
            const char* name,
            uint32_t hash,
            uint32_t checksum,
            uint32_t offset,
            uint32_t length,
            int32_t  timestamp,
            bool     isFile
        );
    	
        Archive&        MGFArchive;
        const QString   Name;
        const uint32_t  FilepathHash;
        const uint32_t  FileChecksum;
        const EFileType FileType;
        const uint32_t  FileOffset;
        const uint32_t  FileLength;
        const QDateTime FileDate;
        const bool      IsFile;
        const Version   ArchiveVersion;

        auto FilePath() const -> const std::filesystem::path&;
        const File* Parent() const;
        const File* ChildAt(size_t at) const;
        const File* SiblingAt(size_t at) const;
        int Row() const;
        size_t GetChildCount() const;
        void AddChild(File* child);
        auto Children() const -> const std::vector<const File*>&;

        // takes a relative file path and traverses the MGF file system to find the desired item
        const File* FindRelativeItem(const std::filesystem::path& relativePath) const;

        static EFileType GetEFileTypeFromExtension(const std::filesystem::path& extension);
    };

}
