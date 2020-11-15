#pragma once

#include "mgf/mgftreeitem.h"

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
		const MGFTreeItem& mgfItem;
		FileExtractStatus status;
	};

} }