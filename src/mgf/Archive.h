#ifndef MGFARCHIVE_H
#define MGFARCHIVE_H

#include "File.h"
#include "models/mgffiletreemodel.h"
#include "models/mgffiletablemodel.h"

#include <QString>

#include <fstream>
#include <exception>
#include <string>

struct MGFFileEntryStruct;

namespace MGF {

    class Archive
    {
    public:
        Archive(const QString& mgfFilePath);

    public:
        inline const QString& GetFileName() const { return m_FileName; }
        inline const QString& GetFilePath() const { return m_FilePath; }
        inline uint32_t GetFileSize() const { return m_FileSize; }
        inline uint32_t GetFileCount() const { return m_FileEntryCount; }
        inline uint8_t GetArchiveVersion() const { return m_ArchiveVersion; }
        inline std::ifstream& FileStream() { return m_FileStream; }
        inline uint32_t NumTextures() const { return m_NumTextures; }
        inline MGFFileTreeModel* FileTreeModel() { return &m_FileTreeModel; }
        inline MGFFileTableModel* FileTableModel() { return &m_FileTableModel; }

    private:
        mutable std::ifstream m_FileStream;

        QString     m_FilePath;
        QString     m_FileName;

        uint32_t    m_FileSize = 0;
        uint8_t     m_ArchiveVersion = 0;
        uint32_t    m_FileEntryCount = 0;
        uint32_t    m_FileEntryLength = 0;
        uint32_t    m_FileEntryOffset = 0;
        uint32_t    m_IndexEntryCount = 0;
        uint32_t    m_IndexEntryLength = 0;
        uint32_t    m_IndexEntryOffset = 0;
        uint32_t    m_StringListLength = 0;
        uint32_t    m_StringListOffset = 0;

        std::vector<MGF::File> m_TreeItems;
        MGFFileTreeModel m_FileTreeModel;
        MGFFileTableModel m_FileTableModel;

        uint32_t m_NumTextures = 0;
    private:
        // helpers
        void LoadMechAssaultFileData(const MGFFileEntryStruct& descriptor);

    private:
        friend class TextureListModel;
    };

}

#endif // MGFARCHIVE_H
