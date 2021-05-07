#pragma once

#include "MGF/Assets/AssetBase.h"

#include "AssetViewers/EntityViewer/Models/EntityTreeModel.h"
#include "AssetViewers/EntityViewer/Models/AttributeTableModel.h"

#include <vector>

namespace MGF { namespace Asset {


	struct Entity
	{
		~Entity() { for (auto c : Children) { delete c; } }

		QString Description;
		QString Class;
		uint32_t UID;
		uint32_t ParentID;
		uint32_t ChildID;
		uint32_t SiblingID;

		EntityViewer::Models::AttributeTable Attributes;

		Entity* ParentEntity = nullptr;
		QVector<Entity*> Children;

		int Row() { return ParentEntity ? ParentEntity->Children.indexOf(this) : 0; }
	};

	class EntityAsset : public AssetBase
	{
		friend class EntityViewer::Models::EntityTree;

	public:
		EntityAsset(const MGF::File& entityFile);
		~EntityAsset() { delete RootEntity; }

		const auto GetEntityTreeModel() { return &EntityTreeModel; }

	private:
		Entity* RootEntity = nullptr;
		EntityViewer::Models::EntityTree EntityTreeModel;
	};
} }