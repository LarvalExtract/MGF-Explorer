#pragma once

#include "Interfaces/IFactory.h"

namespace StringTableViewer { namespace Factories {

	class StringTableFactory : public IFactory
	{
		friend class StringTableModel;

	public:
		QAbstractItemModelPtr Create(const MGFTreeItem& file) override;
	};

} }