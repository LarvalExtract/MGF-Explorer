#pragma once

#include "MGF/MGFFile.h"

namespace FileExtractor { namespace Models {
	
	enum class FileExtractStatus
	{
		Queued,
		Done,
		Skipped,
		Failed
	};

	struct FileExtractItem
	{
		const MGFFile* mgfItem;
		FileExtractStatus status;
	};

} }