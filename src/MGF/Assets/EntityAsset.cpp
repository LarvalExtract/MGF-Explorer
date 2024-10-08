#include "EntityAsset.h"

#include "MGF/Structures/wdf.h"
#include "MGF/Deserializer.h"
#include "Factories/EntityNames.h"
#include "Factories/AttributeNames.h"

#include <unordered_map>

using namespace MGF::Asset;

MGF::Asset::EntityAsset::EntityAsset(const MGF::File& entityFile) :
	AssetBase(entityFile, MGF::Asset::EAssetType::Entity)
{
	size_t offset = 0;

	MGF::Deserializer entityDeserializer(entityFile);

	const auto wdf = entityDeserializer.Deserialize<WDF>();

	std::unordered_map<uint32_t, Entity*> mapEntities;

	for (size_t i = 0; i < wdf.numEntities; i++)
	{
		const auto entity = entityDeserializer.Deserialize<ENTITY>();
		
		auto e = new Entity;
		e->Class = entityFile.FileType == EFileType::wdf
			? GetEntityClassNameWdf(entity.crc)
			: GetEntityClassNameMtb(entity.crc);

		e->Attributes.reserve(entity.numAttributes);

		for (size_t j = 0; j < entity.numAttributes; j++)
		{
			const auto attribute = entityDeserializer.Deserialize<ATTRIBUTE>();
			
			QString a;
			switch (attribute.dataType)
			{
			case 1:
			case 16: a = entityDeserializer.Deserialize<bool>() ? "true" : "false";	break;
			case 3: a = QString::number(entityDeserializer.Deserialize<int32_t>()); break;
			case 4: a = QString::number(entityDeserializer.Deserialize<uint8_t>()); break;
			case 5: a = QString::number(entityDeserializer.Deserialize<uint16_t>()); break;
			case 7: a = QString::number(entityDeserializer.Deserialize<float>()); break;
			case 9:
			{
				std::vector<char> str(attribute.dataLength);
				entityDeserializer.ReadBytes(str.data(), str.size(), entityDeserializer.CurrentOffset());
				a = str.data();
				entityDeserializer.CurrentOffset() += attribute.dataLength;
			} break;
			case 10:
				a = QString("%1, %2").arg(
					QString::number(entityDeserializer.Deserialize<float>()),
					QString::number(entityDeserializer.Deserialize<float>())
				);
				break;
			case 11:
			case 14:
				a = QString("%1, %2, %3").arg(
					QString::number(entityDeserializer.Deserialize<float>()),
					QString::number(entityDeserializer.Deserialize<float>()),
					QString::number(entityDeserializer.Deserialize<float>())
				);
				break;
			case 6: a = QString::number(entityDeserializer.Deserialize<uint32_t>()); break;
			case 12: 
				a = QString("%1, %2, %3, %4").arg(
					QString::number(entityDeserializer.Deserialize<uint8_t>()),
					QString::number(entityDeserializer.Deserialize<uint8_t>()),
					QString::number(entityDeserializer.Deserialize<uint8_t>()),
					QString::number(entityDeserializer.Deserialize<uint8_t>())
				);
				break;
			case 13: 
				a = "TIF"; 
				entityDeserializer.CurrentOffset() += attribute.dataLength;
				break;
			}
			
			auto attributeString = entityFile.FileType == EFileType::wdf
				? GetAttributeNameWdf(attribute.crc)
				: GetAtrributeNameMtb(attribute.crc);

			if (std::strcmp(attributeString, "Description") == 0)
			{
				e->Description = a;
			}
			else if (std::strcmp(attributeString, "UID") == 0)
			{
				e->UID = a.toUInt();
			}
			else if (std::strcmp(attributeString, "parent") == 0)
			{
				e->ParentID = a.toUInt();
			}
			else if (std::strcmp(attributeString, "child") == 0)
			{
				e->ChildID = a.toUInt();
			}
			else if (std::strcmp(attributeString, "sibling") == 0)
			{
				e->SiblingID = a.toUInt();
			}

			e->Attributes.push_back(std::make_tuple(
				attributeString,
				a,
				offset
			));
		}

		mapEntities.emplace(e->UID, e);
	}

	RootEntity = new Entity;
	RootEntity->Children.push_back(mapEntities[wdf.rootEntityUID]);
	mapEntities.at(wdf.rootEntityUID)->ParentEntity = RootEntity;

	for (auto [id, entity] : mapEntities)
	{
		auto parent = entity->ParentID ? mapEntities[entity->ParentID] : nullptr;
		auto child = entity->ChildID ? mapEntities[entity->ChildID] : nullptr;
		auto sibling = entity->SiblingID ? mapEntities[entity->SiblingID] : nullptr;

		if (parent)
		{
			entity->ParentEntity = parent;
			parent->Children.append(entity);
		}
		if (child)
		{
			//child->ParentEntity = entity;
			//entity->Children.append(child);
		}
		if (sibling)
		{
			//sibling->ParentEntity = parent;
			//parent->Children.append(sibling);
		}
	}

	EntityTreeModel.AssetRef = this;
}
