#include "MGFMap.h"
#include "MGFExplorerApplication.h"

#include "Utilities/configfile.h"

#include <QEntity>
#include <QPlaneMesh>
#include <QDiffuseSpecularMaterial>

static const QVariant AttributeTableViewHeaders[] = {
	"Name", "Value", "Offset"
};

int WdfEntity::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return Attributes.size();
}

int WdfEntity::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(AttributeTableViewHeaders) / sizeof(AttributeTableViewHeaders[0]);
}

QVariant WdfEntity::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (!(index.isValid() && role == Qt::DisplayRole))
	{
		return QVariant();
	}

	switch (const Attribute& Attribute = Attributes.at(index.row()); index.column())
	{
	case 0: return Attribute.Name.data();
	case 1: return Attribute.Value;
	case 2: return Attribute.Offset;
	}
}

QVariant WdfEntity::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		? AttributeTableViewHeaders[section]
		: QVariant();
}

MGFMap::MGFMap(const MGFFile& entityFile) :
	MGFAsset(entityFile)
{
	std::filesystem::path filename = FileRef.FilePath().filename();
	std::filesystem::path mgfExtension("mgf");
	filename.replace_extension(mgfExtension);

	// Look in user's MGF folder for MGF archive 
	MapResourceArchive = qApp->GetMgfArchive(filename);


	// If that didn't work, look for the ini file which identifies the MGF archive

	// If that didn't work, open a file dialog and allow the user to find the MGF file

	//if (const MGF::MGFFile* iniFile = FileRef.FindRelativeItem(filename))
	//{
	//	ConfigFile ini(iniFile);
	//	const std::filesystem::path resourceFilename = ini["Mission"]["ResourceFile"];
	//
	//
	//}



	MABinaryObjectParser BinaryObjectParser(entityFile, this);
	BinaryObjectParser.Parse();
}

void MGFMap::OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID)
{
	Objects.reserve(EntityCount);
	this->RootEntityUid = RootEntityUID;
}

void MGFMap::OnObjectRead(const MABinaryObject& MapEntity)
{
	const int32_t UID = std::get<int32_t>(MapEntity.Attributes.at("UID").Value);

	WdfEntity Entity;
	Entity.Class = MapEntity.Name.data();
	Entity.Description = std::get<std::string>(MapEntity.Attributes.at("Description").Value).c_str();
	Entity.UID = UID;
	Entity.ParentUID = std::get<int32_t>(MapEntity.Attributes.at("parent").Value);
	Entity.SiblingUID = std::get<int32_t>(MapEntity.Attributes.at("sibling").Value);
	Entity.ChildUID = std::get<int32_t>(MapEntity.Attributes.at("child").Value);

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
			Attribute.Value = QString("%1, %2").arg(QString::number(vec.x()), QString::number(vec.y()));
		}
		else if (std::holds_alternative<QVector3D>(attrib.Value))
		{
			const QVector3D vec = std::get<QVector3D>(attrib.Value);
			Attribute.Value = QString("%1, %2, %3").arg(QString::number(vec.x()), QString::number(vec.y()), QString::number(vec.z()));
		}
		else if (std::holds_alternative<std::string>(attrib.Value))
		{
			Attribute.Value = QString::fromStdString(std::get<std::string>(attrib.Value));
		}

		Entity.Attributes.emplace_back(Attribute);
	}

	std::sort(Entity.Attributes.begin(), Entity.Attributes.end(), [](const WdfEntity::Attribute& a, const WdfEntity::Attribute& b) {
		for (int i = 0, n = std::min(a.Name.size(), b.Name.size()); i < n; ++i)
		{
			if (std::tolower(a.Name[i]) < std::tolower(b.Name[i]))
			{
				return true;
			}
			else if (std::tolower(a.Name[i]) > std::tolower(b.Name[i]))
			{
				return false;
			}
		}
		return std::tolower(a.Name[0]) < std::tolower(b.Name[0]); 
	});

	if (WdfEntity* Parent = Objects.contains(Entity.ParentUID) ? &Objects.at(Entity.ParentUID) : nullptr)
	{
		std::vector<int32_t>& Children = Parent->Children;
		if (UID == Parent->ChildUID)
		{
			Children.insert(Children.begin(), UID);
		}
		else
		{
			const auto predicate = [UID, this](const int32_t Element) { return UID == Objects[Element].SiblingUID; };

			if (const auto result = std::find_if(Children.begin(), Children.end(), predicate); result != Children.end())
			{
				Children.insert(result + 1, UID);
			}
		}
	}

	Objects[UID] = Entity;

	if (Entity.Class == "MATerrain")
	{
		OnReadEntity_MATerrain(MapEntity);
	}
	else if (Entity.Class == "MATerrainInfo")
	{
		OnReadEntity_MATerrainInfo(MapEntity);
	}
}

#pragma region QAbstractItemModel
static const QVariant EntityTreeViewHeaders[] = {
	"Name", "Class", "ID"
};


QModelIndex MGFMap::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	const WdfEntity& parentEntity = parent.isValid() ? *static_cast<WdfEntity*>(parent.internalPointer()) : Objects.at(RootEntityUid);
	const WdfEntity& childEntity = Objects.at(parentEntity.Children[row]);

	return createIndex(row, column, &childEntity);
}

QModelIndex MGFMap::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	const WdfEntity* pThisEntity = static_cast<WdfEntity*>(child.internalPointer());
	if (Objects.contains(pThisEntity->ParentUID))
	{
		const WdfEntity& ParentEntityRef = Objects.at(pThisEntity->ParentUID);
		const WdfEntity* ParentEntityPtr = &ParentEntityRef;

		const auto it = std::find(ParentEntityRef.Children.begin(), ParentEntityRef.Children.end(), pThisEntity->UID);

		return createIndex(it - ParentEntityRef.Children.begin(), 0, ParentEntityPtr);
	}
	//else if (pThisEntity->UID == MapAsset.RootEntityUid)
	//{
	//	return createIndex(0, 0, &MapAsset.Objects.at(MapAsset.RootEntityUid));
	//}

	return QModelIndex();
}

int MGFMap::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	if (parent.isValid())
	{
		const WdfEntity* entity = static_cast<WdfEntity*>(parent.internalPointer());
		return entity->Children.size();
	}
	else
	{
		const WdfEntity& entity = Objects.at(RootEntityUid);
		return entity.Children.size();
	}
}

int MGFMap::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(EntityTreeViewHeaders) / sizeof(EntityTreeViewHeaders[0]);
}

QVariant MGFMap::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (index.isValid() && role == Qt::DisplayRole)
	{
		if (const WdfEntity* entity = static_cast<WdfEntity*>(index.internalPointer()))
		{
			switch (index.column())
			{
			case 0: return entity->Description.c_str();
			case 1: return entity->Class.c_str();
			case 2: return entity->UID;
			}
		}
	}

	return QVariant();
}

QVariant MGFMap::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? EntityTreeViewHeaders[section] : QVariant();
}
#pragma endregion

void MGFMap::OnReadEntity_MATerrain(const MABinaryObject& MATerrainEntity)
{
	TerrainEntity = new Qt3DCore::QEntity();

	TerrainMesh = new Qt3DExtras::QPlaneMesh(TerrainEntity);
	TerrainMesh->setWidth(512.0f);
	TerrainMesh->setHeight(512.0f);
	TerrainMesh->setMeshResolution(QSize(16, 18));
	TerrainEntity->addComponent(TerrainMesh);

	Qt3DExtras::QDiffuseSpecularMaterial* mat = new Qt3DExtras::QDiffuseSpecularMaterial;
	TerrainEntity->addComponent(mat);
}

void MGFMap::OnReadEntity_MATerrainInfo(const MABinaryObject& MATerrainInfo)
{

}
