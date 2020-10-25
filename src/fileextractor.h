#ifndef FILEEXTRACTOR_H
#define FILEEXTRACTOR_H

#include "mgf/mgftreeitem.h"

#include <QModelIndexList>
#include <QStandardItemModel>

#include <filesystem>
#include <unordered_set>
#include <deque>

enum class FileExtractStatus
{
    Queued,
    Done,
    Skipped,
    Failed
};

struct FileExtractItem
{
    const MGFTreeItem& mgfItem;
    FileExtractStatus status;
};

class FileExtractor
{
    friend class FileExtractListModel;

public:
    void SetDestinationFolder(const std::filesystem::path& folder);
    void SetFileQueueFromSelection(const QModelIndexList& items);
    void ExtractFiles(int* progress, bool bOverwriteExistingFiles = false);
    std::size_t NumFilesInQueue() const { return m_FileQueue.size(); }

private:
    static constexpr std::size_t FILE_BUFFER_SIZE = 16384;
    std::vector<FileExtractItem> m_FileQueue;
    std::filesystem::path m_Destination;

    void TraverseTreeItem(const MGFTreeItem* item);
    void CreateFile(FileExtractItem& item, std::vector<char>& buf);
};

#endif // FILEEXTRACTOR_H
