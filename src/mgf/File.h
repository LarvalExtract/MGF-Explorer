#ifndef MGFTREEITEM_H
#define MGFTREEITEM_H

#include "EFileTypes.h"
#include "VersionConstants.h"

#include <QString>
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

        inline File* GetParent() const { return m_Parent; }
        const File* GetNthChild(int index) const;
        const File* GetNamedChild(const QString& name) const;
        const File* GetNamedSibling(const QString& name) const;
        const auto& GetChildren() const { return m_ChildrenArray; }

        inline int Row() const { return m_TreeViewRow; }

        inline const QString& Name() const { return m_Name; }
        inline const std::filesystem::path& FilePath() const { return m_FilePath; }
        inline uint64_t GUID() const { return m_GUID; }
        inline uint32_t Index() const { return m_Index; }
        inline EFileType FileType() const { return m_FileType; }
        inline uint32_t FileOffset() const { return m_FileOffset; }
        inline uint32_t FileLength() const { return m_FileLength; }
        inline QDateTime FileDate() const { return m_FileDate; }
        inline bool IsFile() const { return m_IsFile; }

        inline const MGF::Archive& Archive() const { return m_MGFArchive; }
        const MGF::Version GetArchiveVersion() const;
        void LoadBuffer(std::string& out, unsigned int offset = 0, int length = -1) const;

        void AddChildItem(const File* item);
        size_t GetChildCount() const;

        // takes a relative file path and traverses the MGF file system to find the desired item
        const MGF::File* FindRelativeItem(const QString& relativePath) const;

        void Read(char* buffer, ::uint32_t offset = 0, std::uint32_t length = INT_MAX) const;

    private:
        File* m_Parent;
        QHash<QString, const File*> m_Children;
        std::vector<const File*> m_ChildrenArray;

        std::filesystem::path m_FilePath;
        QString       m_Name;
        uint64_t      m_GUID;
        uint32_t      m_Index;
        MGF::EFileType m_FileType;
        uint32_t      m_FileOffset;
        uint32_t      m_FileLength;
        QDateTime     m_FileDate;
        bool          m_IsFile;

        MGF::Archive& m_MGFArchive;
        int m_TreeViewRow;

    private:
        static const std::unordered_map<std::string, MGF::EFileType>  MapExtensionFileType;
    };

}
#endif // MGFTREEITEM_H
