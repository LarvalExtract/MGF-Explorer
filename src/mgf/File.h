#pragma once

#include "EFileTypes.h"
#include "VersionConstants.h"

#include <QDateTime>

#include <filesystem>
#include <unordered_map>

namespace MGF {

    class Archive;

    class File
    {
        friend class Archive;
    	
        const int32_t _Index = 0;
        const int32_t _ParentIndex = 0;
        const int32_t _SiblingIndex = 0;
        const int32_t _ChildIndex = 0;
        uint32_t _ChildCount = 0;
    
    public:
        File(
            int32_t  index,
            int32_t  parentIndex,
            int32_t  siblingIndex,
            int32_t  childIndex,
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
        const std::filesystem::path FilePath;
        const QString   Name;
        const uint32_t  FilepathHash;
        const uint32_t  FileChecksum;
        const EFileType FileType;
        const uint32_t  FileOffset;
        const uint32_t  FileLength;
        const QDateTime FileDate;
        const bool      IsFile;
        const Version   ArchiveVersion;

        const File* Parent() const;
        const File* Sibling() const;
        const File* Child() const;
        const File* ChildAt(size_t at = 0) const;
        const File* SiblingAt(size_t at = 0) const;
        int GetChildCount() const { return _ChildCount; }

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

        static const std::unordered_map<std::string, EFileType>  MapExtensionFileType;
    };

}
