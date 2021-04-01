#pragma once

#include "MGF/File.h"

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
		const MGF::File* mgfItem;
		FileExtractStatus status;
	};

} }