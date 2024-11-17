#pragma once

#include "MGF/Assets/AssetBase.h"

#include "MGF/Factories/MABinaryObjectParser.h"

#include "AssetViewers/EntityViewer/Models/EntityTreeModel.h"
#include "AssetViewers/EntityViewer/Models/AttributeTableModel.h"

namespace MGF { namespace Asset {

	struct WdfEntity
	{
		std::string Class;
		std::string Description;
		int32_t UID = 0;
		int32_t ParentUID = 0;
		int32_t SiblingUID = 0;
		int32_t ChildUID = 0;

		struct Attribute
		{
			std::string_view Name;
			QString Value;
			size_t Offset;
			size_t Length;
		};
		QVector<Attribute> Attributes;

		WdfEntity* Parent = nullptr;
		std::vector<WdfEntity*> Children;
	};

	class WdfMap : public AssetBase, public IListener
	{
		friend class EntityViewer::Models::EntityTreeModel;

	public:
		WdfMap(const MGF::File& entityFile);

		void OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID) override;
		void OnObjectRead(const MABinaryObject& MapEntity) override;
		EntityViewer::Models::EntityTreeModel& GetEntityTreeModel();

	private:
		uint32_t RootEntityUid = 0;
		std::unordered_map<int32_t, WdfEntity> Objects;

		EntityViewer::Models::EntityTreeModel EntityTreeModel;
	};
} }