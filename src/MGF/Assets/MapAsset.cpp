#include "MapAsset.h"

using namespace MGF::Asset;

MGF::Asset::WdfMap::WdfMap(const MGF::File& entityFile) :
	AssetBase(entityFile, MGF::Asset::EAssetType::Entity),
	EntityTreeModel(*this)
{
	MABinaryObjectParser BinaryObjectParser(entityFile, this);
	BinaryObjectParser.Parse();
}

void MGF::Asset::WdfMap::OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID)
{
	Objects.reserve(EntityCount);
	this->RootEntityUid = RootEntityUID;
}

void MGF::Asset::WdfMap::OnObjectRead(const MABinaryObject& MapEntity)
{
	const int32_t UID = std::get<int32_t>(MapEntity.Attributes.at("UID").Value);

	Objects.emplace(UID, WdfEntity{
		.Class = MapEntity.Name.data(),
		.Description = std::get<std::string>(MapEntity.Attributes.at("Description").Value).c_str(),
		.UID = UID,
		.ParentUID = std::get<int32_t>(MapEntity.Attributes.at("parent").Value),
		.SiblingUID = std::get<int32_t>(MapEntity.Attributes.at("sibling").Value),
		.ChildUID = std::get<int32_t>(MapEntity.Attributes.at("child").Value)
	});

	WdfEntity& Entity = Objects.at(UID);

	Entity.Attributes.reserve(MapEntity.Attributes.size());
	for (const auto& [name, attrib] : MapEntity.Attributes)
	{
		WdfEntity::Attribute Attribute;
		Attribute.Name = name;
		Attribute.Offset = attrib.Offset;
		Attribute.Length = attrib.Length;

		if (std::holds_alternative<bool>(attrib.Value))
		{
			Attribute.Value = std::get<bool>(attrib.Value) ? "true" : "false";
		}
		else if (std::holds_alternative<uint8_t>(attrib.Value))
		{
			Attribute.Value = QString::number(std::get<uint8_t>(attrib.Value));
		}
		else if (std::holds_alternative<uint16_t>(attrib.Value))
		{
			Attribute.Value = QString::number(std::get<uint16_t>(attrib.Value));
		}
		else if (std::holds_alternative<int32_t>(attrib.Value))
		{
			Attribute.Value = QString::number(std::get<int32_t>(attrib.Value));
		}
		else if (std::holds_alternative<QColor>(attrib.Value))
		{
			Attribute.Value = std::get<QColor>(attrib.Value).name(QColor::NameFormat::HexArgb);
		}
		else if (std::holds_alternative<float>(attrib.Value))
		{
			Attribute.Value = QString::number(std::get<float>(attrib.Value));
		}
		else if (std::holds_alternative<QVector2D>(attrib.Value))
		{
			const QVector2D vec = std::get<QVector2D>(attrib.Value);
			Attribute.Value = QString("%1, %2").arg(vec.x(), vec.y());
		}
		else if (std::holds_alternative<QVector3D>(attrib.Value))
		{
			const QVector3D vec = std::get<QVector3D>(attrib.Value);
			Attribute.Value = QString("%1, %2, %3").arg(vec.x(), vec.y(), vec.z());
		}
		else if (std::holds_alternative<std::string>(attrib.Value))
		{
			Attribute.Value = QString::fromStdString(std::get<std::string>(attrib.Value));
		}

		Entity.Attributes.emplaceBack(Attribute);
	}

	if (Entity.Parent = Objects.contains(Entity.ParentUID) ? &Objects.at(Entity.ParentUID) : nullptr)
	{
		std::vector<WdfEntity*>& Children = Entity.Parent->Children;
		if (UID == Entity.Parent->ChildUID)
		{
			Children.insert(Children.begin(), &Entity);
		}
		else
		{
			const auto predicate = [UID](WdfEntity* Element) { return UID == Element->SiblingUID; };

			if (const auto result = std::find_if(Children.begin(), Children.end(), predicate); result != Children.end())
			{
				Children.insert(result + 1, &Entity);
			}
		}
	}
	
}

EntityViewer::Models::EntityTreeModel& MGF::Asset::WdfMap::GetEntityTreeModel() 
{
	return EntityTreeModel;
}
