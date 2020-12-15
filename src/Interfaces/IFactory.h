#pragma once

#include <QAbstractItemModel>

#include <memory>

class MGFTreeItem;

using QAbstractItemModelPtr = std::shared_ptr<QAbstractItemModel>;

class IFactory
{
public:
	virtual QAbstractItemModelPtr Create(const MGFTreeItem& file) = 0;
};