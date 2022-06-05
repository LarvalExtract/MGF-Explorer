#include "Extractor.h"
#include "MGF/Archive.h"

#include <fstream>

using namespace FileExtractor;

void Extractor::ExtractFiles(
    Models::FileExtractListModel& items,
	const std::filesystem::path& destination,
	bool bOverwriteExistingFiles,
	unsigned int& numFilesExtracted)
{
    auto createFile = [&destination,&numFilesExtracted](const std::filesystem::path& path, Models::FileExtractItem& item, std::vector<char>& buf)
    {
		std::filesystem::create_directories(path.parent_path());
		std::ofstream file(path, std::ios::binary);

		if (file.fail())
		{
			return Models::FileExtractStatus::Failed;
		}

		std::int32_t offset = 0;
		std::int32_t remainingBytes = item.mgfItem->FileLength;

		while (remainingBytes > 0)
		{
			const std::int32_t bytesToCopy = std::min<int>(buf.capacity(), remainingBytes);
			item.mgfItem->Read(buf.data(), offset, bytesToCopy);
			file.write(buf.data(), bytesToCopy);

			remainingBytes -= buf.capacity();
			offset += buf.capacity();
		}
    	
		numFilesExtracted++;
		return Models::FileExtractStatus::Done;
    };

    std::vector<char> buffer(FILE_BUFFER_SIZE);
	
	if (bOverwriteExistingFiles)
	{
		for (auto& item : items)
        {
			auto path = destination;
			path += item.mgfItem->FilePath;
            createFile(path, item, buffer);
		}
	}
	else
	{
		for (auto& item : items)
		{
			auto path = destination;
			path += item.mgfItem->FilePath;
			item.status = std::filesystem::exists(path)
				? Models::FileExtractStatus::Skipped
				: createFile(path, item, buffer);
		}
	}
}

void TraverseTreeItem(std::vector<Models::FileExtractItem>& list, const MGF::File* item)
{
	if (item->IsFile)
	{
		list.push_back({ item, Models::FileExtractStatus::Queued });
	}
	else
	{
		for (item = item->Child(); item != nullptr; item = item->Sibling())
		{
			TraverseTreeItem(list, item);
		}
	}
}

std::vector<Models::FileExtractItem> Extractor::ToList(const QModelIndexList& selection)
{
	if (selection.isEmpty())
	{
		
	}
	
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

bool Extractor::WriteFile(const MGF::File& item, const std::filesystem::path& destination, std::vector<char>& buf)
{
	std::filesystem::create_directories(destination.parent_path());
	std::ofstream file(destination, std::ios::binary);

	if (file.fail())
	{
		return false;
	}

	std::int32_t offset = 0;
	std::int32_t remainingBytes = item.FileLength;
	std::size_t bufSize = buf.capacity();
	std::int32_t bytesToCopy;

	while (remainingBytes > 0)
	{
		bytesToCopy = std::min<int>(bufSize, remainingBytes);
		item.Read(buf.data(), offset, bytesToCopy);
		file.write(buf.data(), bytesToCopy);

		remainingBytes -= bufSize;
		offset += bufSize;
	}

	return true;
}
