#pragma once

#include "MGF/Assets/AssetBase.h"

#include "MGF/Factories/MABinaryObjectParser.h"

#include "AssetViewers/EntityViewer/Models/EntityTreeModel.h"

namespace Qt3DCore
{
	class QEntity;
}

namespace Qt3DExtras
{
	class QPlaneMesh;
}

namespace MGF { namespace Asset {

	struct WdfEntity : public QAbstractTableModel
	{
		WdfEntity() = default;
		WdfEntity(const WdfEntity&) {}
		WdfEntity& operator=(const WdfEntity& other) 
		{
			this->Class = other.Class;
			this->Description = other.Description;
			this->UID = other.UID;
			this->ParentUID = other.ParentUID;
			this->SiblingUID = other.SiblingUID;
			this->ChildUID = other.ChildUID;
			this->Attributes = other.Attributes;
			this->Children = other.Children;

			return *this;
		}

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
		std::vector<Attribute> Attributes;
		std::vector<int32_t> Children;

	public:
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	};

	class WdfMap : public AssetBase, public IListener
	{
		friend class EntityViewer::Models::EntityTreeModel;

	public:
		WdfMap(const MGF::File& entityFile);

		void OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID) override;
		void OnObjectRead(const MABinaryObject& MapEntity) override;

		EntityViewer::Models::EntityTreeModel& GetEntityTreeModel();

		Qt3DCore::QEntity* TerrainEntity = nullptr;

	private:
		int32_t RootEntityUid = 0;
		std::unordered_map<int32_t, WdfEntity> Objects;

		EntityViewer::Models::EntityTreeModel EntityTreeModel;

		void OnReadEntity_MATerrain(const MABinaryObject& MATerrainEntity);
		void OnReadEntity_MATerrainInfo(const MABinaryObject& MATerrainInfo);

		Qt3DExtras::QPlaneMesh* TerrainMesh = nullptr;
	};
} }