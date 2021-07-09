#include "EntityAsset.h"

#include "MGF/Structures/wdf.h"
#include "Factories/EntityNames.h"
#include "Factories/AttributeNames.h"

#include <unordered_map>

using namespace MGF::Asset;

MGF::Asset::EntityAsset::EntityAsset(const MGF::File& entityFile) :
	AssetBase(entityFile, MGF::Asset::EAssetType::Entity)
{
	size_t offset = 0;

	const auto wdf = entityFile.Read<WDF>(offset);
	offset += sizeof(wdf);

	std::unordered_map<uint32_t, Entity*> mapEntities;

	for (size_t i = 0; i < wdf.numEntities; i++)
	{
		const auto entity = entityFile.Read<ENTITY>(offset);
		offset += sizeof(entity);
		
		auto e = new Entity;
		e->Class = entityFile.FileType == EFileType::wdf
			? GetEntityClassNameWdf(entity.crc)
			: GetEntityClassNameMtb(entity.crc);

		e->Attributes.reserve(entity.numAttributes);

		for (size_t j = 0; j < entity.numAttributes; j++)
		{
			const auto attribute = entityFile.Read<ATTRIBUTE>(offset);
			offset += sizeof(attribute);
			
			QString a;
			switch (attribute.dataType)
			{
			case 1:
			case 16:
				a = entityFile.Read<bool>(offset) ? "true" : "false"; 
				break;
			case 3: a = QString::number(entityFile.Read<int32_t>(offset)); break;
			case 4: a = QString::number(entityFile.Read<uint8_t>(offset)); break;
			case 5: a = QString::number(entityFile.Read<uint16_t>(offset)); break;
			case 7: a = QString::number(entityFile.Read<float>(offset)); break;
			case 9:
			{
				char* str = new char[attribute.dataLength];
				entityFile.Read(str, offset, attribute.dataLength);
				a = str;
				delete[] str;
			} break;
			case 10:
			{
				a = QString("%1, %2").arg(
					QString::number(entityFile.Read<float>(offset)),
					QString::number(entityFile.Read<float>(offset + 4))
				);
			} break;
			case 11:
			case 14:
			{
				a = QString("%1, %2, %3").arg(
					QString::number(entityFile.Read<float>(offset)),
					QString::number(entityFile.Read<float>(offset + 4)),
					QString::number(entityFile.Read<float>(offset + 8))
				);
			} break;
			case 6: a = QString::number(entityFile.Read<uint32_t>(offset)); break;
			case 12: 
			{
				a = QString("%1, %2, %3, %4").arg(
					QString::number(entityFile.Read<uint8_t>(offset)),
					QString::number(entityFile.Read<uint8_t>(offset + 1)),
					QString::number(entityFile.Read<uint8_t>(offset + 2)),
					QString::number(entityFile.Read<uint8_t>(offset + 3))
				);
			} break;
			case 13: a = "TIF"; break;
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

			offset += attribute.dataLength;
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
