#include "MapAsset.h"

using namespace MGF::Asset;

static const QVariant HEADERS[] = {
	"Name", "Value", "Offset"
};

using namespace EntityViewer::Models;
using namespace MGF::Asset;

int WdfEntity::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return Attributes.size();
}

int WdfEntity::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return sizeof(HEADERS) / sizeof(HEADERS[0]);
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
		? HEADERS[section]
		: QVariant();
}

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

		Entity.Attributes.emplace_back(Attribute);
	}

	std::sort(Entity.Attributes.begin(), Entity.Attributes.end(), [](const WdfEntity::Attribute& a, const WdfEntity::Attribute& b) {
		for (int i = 0, n = std::min(a.Name.size(), b.Name.size()); i < n; ++i)
		{
			if (a.Name[i] < b.Name[i])
			{
				return true;
			}
			else if (a.Name[i] > b.Name[i])
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
}

EntityViewer::Models::EntityTreeModel& MGF::Asset::WdfMap::GetEntityTreeModel() 
{
	return EntityTreeModel;
}
