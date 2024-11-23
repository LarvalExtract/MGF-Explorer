#include "IContextMenu.h"
#include "MGF/MGFArchive.h"
#include "MGF/MGFFile.h"

using namespace ContextMenus;
using namespace FileExtractor::Models;

void TraverseTreeItem(FileItemList&, const MGFFile*);

FileItemList IContextMenu::SelectedItems() const
{
	const auto indexes = SelectionModel->selectedRows(0);
	
	FileItemList selectedItems;
	selectedItems.reserve(indexes.count());

	for (const auto& item : indexes)
	{
		if (item.column() != 0)
			continue;

		const auto mgfItem = static_cast<MGFFile*>(item.internalPointer());

		TraverseTreeItem(selectedItems, mgfItem);
	}

	return selectedItems;
}

FileExtractor::Models::FileItemList IContextMenu::AllItems(const MGFArchive& archive) const
{
	FileItemList items;
	items.reserve(archive.GetFileCount());

	TraverseTreeItem(items, archive.Root());

	return items;
}

void TraverseTreeItem(FileExtractListModel& list, const MGFFile* item)
{
	if (item->IsFile)
	{
		list.push_back({ item, FileExtractStatus::Queued });
	}
	else
	{
		for (const auto child : item->Children())
		{
			TraverseTreeItem(list, child);
		}
	}
}
