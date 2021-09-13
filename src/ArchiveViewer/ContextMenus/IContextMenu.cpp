#include "IContextMenu.h"
#include "MGF/Archive.h"

using namespace ArchiveViewer::ContextMenus;
using namespace FileExtractor::Models;

void TraverseTreeItem(FileItemList&, const MGF::File*);

FileItemList IContextMenu::SelectedItems() const
{
	const auto indexes = SelectionModel->selectedRows(0);
	
	FileItemList selectedItems;
	selectedItems.reserve(indexes.count());

	for (const auto& item : indexes)
	{
		if (item.column() != 0)
			continue;

		const auto mgfItem = static_cast<MGF::File*>(item.internalPointer());

		TraverseTreeItem(selectedItems, mgfItem);
	}

	return selectedItems;
}

FileExtractor::Models::FileItemList IContextMenu::AllItems(const MGF::Archive& archive) const
{
	FileItemList items;
	items.reserve(archive.GetFileCount());

	TraverseTreeItem(items, archive.Root());

	return items;
}

void TraverseTreeItem(FileExtractListModel& list, const MGF::File* item)
{
	if (item->IsFile)
	{
		list.push_back({ item, FileExtractStatus::Queued });
	}
	else
	{
		for (item = item->Child(); item != nullptr; item = item->Sibling())
		{
			TraverseTreeItem(list, item);
		}
	}
}
