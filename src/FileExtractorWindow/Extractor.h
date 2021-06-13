#ifndef FILEEXTRACTOR_H
#define FILEEXTRACTOR_H

#include "FileExtractorWindow/Models/FileExtractListModel.h"

#include <QModelIndexList>

#include <filesystem>
#include <functional>

namespace FileExtractor {

	struct ExtractRequest
	{
		const MGF::File& File;
		const std::filesystem::path& Destination;

	};

	class Extractor
	{
		friend class FileExtractListModel;

		static constexpr std::size_t FILE_BUFFER_SIZE = 16384;

	public:
		Extractor() = delete;
		Extractor(const Extractor&) = delete;
		Extractor(Extractor&&) = delete;
		Extractor& operator=(const Extractor&) = delete;
		Extractor& operator=(Extractor&&) = delete;

		static void ExtractFiles(
			Models::FileExtractListModel& items,
			const std::filesystem::path& destination,
			bool bOverwriteExistingFiles,
			const std::function<void(int)>& onFileExtracted,
			const std::function<void()>& onAllFilesExtracted);

		static std::vector<Models::FileExtractItem> ToList(const QModelIndexList& selection);
	};

}
#endif // FILEEXTRACTOR_H
