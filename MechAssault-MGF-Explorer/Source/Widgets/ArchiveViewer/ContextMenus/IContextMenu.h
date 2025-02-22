#pragma once

#include "Windows/FileExtractorWindow/Models/FileExtractListModel.h"

#include <QMenu>
#include <QItemSelection>
#include <QAbstractItemView>

namespace ContextMenus 
{
	class IContextMenu : public QMenu
	{
	public:
		const QItemSelectionModel* SelectionModel = nullptr;
	
	protected:
		FileExtractor::Models::FileItemList SelectedItems() const;
		FileExtractor::Models::FileItemList AllItems(const MGFArchive& archive) const;
	};
}
