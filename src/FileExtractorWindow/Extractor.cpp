#include "Extractor.h"
#include "mgf/Archive.h"

#include <fstream>

using namespace FileExtractor;

void Extractor::ExtractFiles(
    Models::FileExtractListModel& items,
	const std::filesystem::path& destination,
	bool bOverwriteExistingFiles,
	const std::function<void(int)>& onFileExtracted,
	const std::function<void()>& onAllFilesExtracted)
{
    auto createFile = [&destination](Models::FileExtractItem& item, std::vector<char>& buf) -> void
    {
		auto path = destination;
		path += item.mgfItem->FilePath();

		std::filesystem::create_directories(path.parent_path());
		std::ofstream file(path, std::ios::binary);

		if (file.fail())
		{
			item.status = Models::FileExtractStatus::Failed;
			return;
		}

		std::int32_t offset = 0;
		std::int32_t remainingBytes = item.mgfItem->FileLength();
		std::size_t bufSize = buf.capacity();
		std::int32_t bytesToCopy;

		while (remainingBytes > 0)
		{
			bytesToCopy = std::min<int>(bufSize, remainingBytes);
			item.mgfItem->Read(buf.data(), offset, bytesToCopy);
			file.write(buf.data(), bytesToCopy);

			remainingBytes -= bufSize;
			offset += bufSize;
		}

		file.close();
		item.status = Models::FileExtractStatus::Done;
    };

    std::vector<char> buffer(FILE_BUFFER_SIZE);

	unsigned int extractedFileCount = 0;
	if (bOverwriteExistingFiles)
	{
		for (auto& item : items)
        {
            createFile(item, buffer);
			onFileExtracted(++extractedFileCount);
		}
	}
	else
	{
		for (auto& item : items)
		{
            if (!std::filesystem::exists(item.mgfItem->FilePath()))
                createFile(item, buffer);
            else
                item.status = Models::FileExtractStatus::Skipped;

			onFileExtracted(++extractedFileCount);
		}
	}

	onAllFilesExtracted();
}

void TraverseTreeItem(std::vector<Models::FileExtractItem>& list, const MGF::File* item)
{
	if (item->IsFile())
	{
		list.push_back({ item, Models::FileExtractStatus::Queued });
	}
	else
	{
		for (const auto child : item->GetChildren())
			TraverseTreeItem(list, child);
	}
}

std::vector<Models::FileExtractItem> Extractor::ToList(const QModelIndexList& selection)
{
    std::vector<Models::FileExtractItem> result;
    result.reserve(selection.count());

	for (const auto& item : selection)
	{
		if (item.column() != 0)
			continue;

		const auto mgfitem = static_cast<MGF::File*>(item.internalPointer());

		TraverseTreeItem(result, mgfitem);
	}

    return result;
}
