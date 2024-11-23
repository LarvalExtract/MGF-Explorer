#pragma once

#include <QDateTime>

#include <filesystem>
#include <vector>

class MGFArchive;
enum class MGFArchiveVersion : char;

class MGFFile
{
    friend class MGFArchive;

private:
    MGFFile* m_Parent = nullptr;
    std::vector<const MGFFile*> m_Children;
    int m_Row = 0;
    std::filesystem::path m_FilePath;

public:
    MGFFile(
        MGFArchive& mgfFile,
        const char* name,
        uint32_t hash,
        uint32_t checksum,
        uint32_t offset,
        uint32_t length,
        int32_t  timestamp,
        bool     isFile
    );
	
    MGFArchive&     MgfArchive;
    const QString   Name;
    const uint32_t  FilepathHash;
    const uint32_t  FileChecksum;
    const uint32_t  FileOffset;
    const uint32_t  FileLength;
    const QDateTime FileDate;
    const bool      IsFile;

    auto FilePath() const -> const std::filesystem::path&;
    const MGFFile* Parent() const;
    const MGFFile* ChildAt(size_t at) const;
    const MGFFile* SiblingAt(size_t at) const;
    int Row() const;
    size_t GetChildCount() const;
    void AddChild(MGFFile* child);
    const std::vector<const MGFFile*>& Children() const;

    friend inline bool operator==(const MGFFile& a, const MGFFile& b)
    {
        return a.FilepathHash == b.FilepathHash;
    }

    // takes a relative file path and traverses the MGF file system to find the desired item
    const MGFFile* FindRelativeItem(const std::filesystem::path& relativePath) const;

    friend QDebug& operator<<(QDebug& debug, const MGFFile& mgfFile);
};



