#ifndef MGFARCHIVE_H
#define MGFARCHIVE_H

#include "File.h"
#include "VersionConstants.h"

#include <QString>

#include <fstream>
#include <string>

namespace MGF {

    class Archive
    {
    public:
        Archive(const QString& mgfFilePath);

    public:
        inline const QString& GetFileName() const { return m_FileName; }
        inline const QString& GetFilePath() const { return m_FilePath; }
        inline uint32_t GetFileSize() const { return FileSize; }
        inline uint32_t GetFileCount() const { return FileCount; }
        inline MGF::Version GetArchiveVersion() const { return ArchiveVersion; }
        inline std::ifstream& FileStream() { return m_FileStream; }

    private:
        mutable std::ifstream m_FileStream;

        QString m_FilePath;
        QString m_FileName;

        uint32_t     FileSize = 0;
        MGF::Version ArchiveVersion = 0;
		uint32_t     FileCount = 0;

        std::vector<MGF::File> m_TreeItems;
    };

}

#endif // MGFARCHIVE_H
