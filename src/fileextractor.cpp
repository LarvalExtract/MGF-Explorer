#include "fileextractor.h"
#include "mgf/mgfarchive.h"

#include <fstream>

void FileExtractor::SetDestinationFolder(const std::filesystem::path &folder)
{
    m_Destination = folder;
}

void FileExtractor::SetFileQueueFromSelection(const QModelIndexList &items)
{
    for (const auto& item : items)
    {
        if (item.column() != 0)
            continue;

        auto mgfitem = static_cast<MGFTreeItem*>(item.internalPointer());

        TraverseTreeItem(mgfitem);
    }
}

void FileExtractor::ExtractFiles(int* progress, bool bOverwriteExistingFiles)
{
    std::vector<char> buffer;
    buffer.reserve(FILE_BUFFER_SIZE);

    (*progress) = 0;

	if (bOverwriteExistingFiles)
	{
		for (auto& mgfItem : m_FileQueue)
        {
            CreateFile(mgfItem, buffer);
            (*progress)++;
		}
	}
	else
	{
		for (auto& item : m_FileQueue)
		{
            if (!std::filesystem::exists(item.mgfItem.FilePath()))
                CreateFile(item, buffer);
            else
                item.status = FileExtractStatus::Skipped;
            (*progress)++;
		}
	}
}

void FileExtractor::TraverseTreeItem(const MGFTreeItem *item)
{
    if (item->IsFile())
    {
        m_FileQueue.push_back({*item, FileExtractStatus::Queued});
    }
    else
    {
        for (const auto child : item->GetChildren())
            TraverseTreeItem(child);
    }
}

void FileExtractor::CreateFile(FileExtractItem& item, std::vector<char>& buf)
{
    auto path = m_Destination;
    path += item.mgfItem.FilePath();

    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::binary);

    if (file.fail())
    {
        item.status = FileExtractStatus::Failed;
        return;
    }

    std::int32_t offset = 0;
    std::int32_t remainingBytes = item.mgfItem.FileLength();
    std::size_t bufSize = buf.capacity();
    std::int32_t bytesToCopy;

    while (remainingBytes > 0)
    {
        bytesToCopy = std::min<int>(bufSize, remainingBytes);
        item.mgfItem.Read(buf, offset, remainingBytes);
        file.write(buf.data(), bytesToCopy);

        remainingBytes -= bufSize;
        offset += bufSize;
    }

    file.close();
    item.status = FileExtractStatus::Done;
}
