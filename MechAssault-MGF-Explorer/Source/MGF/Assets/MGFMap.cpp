#include "MGFMap.h"
#include "MGFExplorerApplication.h"
#include "MGF/Deserializer.h"
#include "MGFTexture.h"
#include "MGFModel.h"
#include "Render/TextureLibrary.h"

#include "Utilities/configfile.h"

#include <Qt3DCore/QTransform>


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
	if (MapResourceArchive)
	{
		bIsMap = true;
		MapRoot = new Qt3DCore::QEntity;
	}

	MABinaryObjectParser BinaryObjectParser(entityFile, this);
	BinaryObjectParser.Parse();
}

void MGFMap::OnHeaderRead(uint32_t EntityCount, uint32_t RootEntityUID) noexcept
{
	this->RootEntityUid = RootEntityUID;

	WdfEntity FakeRoot;
	FakeRoot.UID = 0;
	FakeRoot.ParentUID = -1;
	FakeRoot.ChildUID = RootEntityUid;

	Objects.reserve(EntityCount + 1);
	Objects[0] = FakeRoot;
}

void MGFMap::OnObjectRead(const MABinaryObject& MapEntity) noexcept
{
	const int32_t UID = std::get<int32_t>(MapEntity.Attributes.at("UID").Value);

	WdfEntity Entity;
	Entity.Class = MapEntity.Name.data();
	Entity.Description = std::get<std::string>(MapEntity.Attributes.at("Description").Value);
	Entity.UID = UID;
	Entity.ParentUID = std::get<int32_t>(MapEntity.Attributes.at("parent").Value);
	Entity.SiblingUID = std::get<int32_t>(MapEntity.Attributes.at("sibling").Value);
	Entity.ChildUID = std::get<int32_t>(MapEntity.Attributes.at("child").Value);

	const auto ToWdfEntity = [](const MABinaryObjectAttribute& attrib) -> WdfEntity::Attribute
		{
			WdfEntity::Attribute Attribute;
			Attribute.Name = attrib.Name;
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

			return Attribute;
		};

	Entity.Attributes.reserve(MapEntity.Attributes.size());
	for (const auto& [name, attrib] : MapEntity.Attributes)
	{
		Entity.Attributes.emplace_back(ToWdfEntity(attrib));
	}

	for (const MABinaryObjectAttribute& unknownAttrib : MapEntity.UnknownAttributes)
	{
		Entity.Attributes.emplace_back(ToWdfEntity(unknownAttrib));
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

	if (bIsMap)
	{
		if (Entity.Class == "MATerrain")
		{
			OnReadEntity_MATerrain(MapEntity);
		}
		else if (Entity.Class == "MATerrainInfo")
		{
			OnReadEntity_MATerrainInfo(MapEntity);
		}
		else if (Entity.Class == "MACorporeal" || Entity.Class == "MABuilding" || Entity.Class == "MAMech")
		{
			OnReadEntity_MACorporeal(MapEntity);
		}
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

	const WdfEntity& parentEntity = parent.isValid() ? *static_cast<WdfEntity*>(parent.internalPointer()) : Objects.at(0);
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
		const WdfEntity& entity = Objects.at(0);
		return entity.Children.size();
	}

	return 0;
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
	using namespace Qt3DCore;
	using namespace Qt3DRender;
	using namespace Qt3DExtras;

	if (MapResourceArchive->IsMechAssault2() && static_cast<int32_t>(MATerrainEntity["TerrainVisible"]) == 0)
	{
		return;
	}

	TerrainEntity = new MATerrain(MapRoot);
	TerrainEntity->SetDetailBlend(MATerrainEntity["TerrainDetailBlend"]);
	TerrainEntity->SetDetailMultiplier0(MATerrainEntity["TerrainDetailMult"]);
	TerrainEntity->SetDetailMultiplier1(MATerrainEntity["TerrainDetailMult1"]);
	TerrainEntity->SetMinHeight(MATerrainEntity["TerrainMinHeight"]);
	TerrainEntity->SetMaxHeight(MATerrainEntity["TerrainMaxHeight"]);
	TerrainEntity->SetTerrainCellSize(MATerrainEntity["TerrainCellSize"]);
	TerrainEntity->SetTerrainWorldScale(MATerrainEntity["TerrainWorldScale"]);
	TerrainEntity->SetTerrainPosition(QVector3D(0.0f, 0.0f, 0.0f));

	const std::string baseMapTexturePath = MATerrainEntity["TerrainBaseMap"];
	const std::string detailMapTexture0Path = MATerrainEntity["TerrainDetailMap"];
	const std::string detailMapTexture1Path = MATerrainEntity["TerrainDetailMap 1"];

	std::shared_ptr<MGFTexture> baseMapTextureAsset = MapResourceArchive->LoadAsset<MGFTexture>(baseMapTexturePath);
	std::shared_ptr<MGFTexture> detailMap0TextureAsset = MapResourceArchive->LoadAsset<MGFTexture>(detailMapTexture0Path);
	std::shared_ptr<MGFTexture> detailMap1TextureAsset = MapResourceArchive->LoadAsset<MGFTexture>(detailMapTexture1Path);

	const MABinaryObjectAttribute terrainHeightMapAttribute = MATerrainEntity["TerrainHeight Field"];
	QAbstractTexture* terrainHeightMapTexture = MGF::Render::TextureLibrary::Get().GetTexture(FileRef, terrainHeightMapAttribute.Offset);
	terrainHeightMapTexture->setMagnificationFilter(QAbstractTexture::Filter::Linear);
	terrainHeightMapTexture->setMagnificationFilter(QAbstractTexture::Filter::Linear);
	terrainHeightMapTexture->setMipLevels(1);
	terrainHeightMapTexture->setWrapMode(QTextureWrapMode(QTextureWrapMode::Repeat));

	if (baseMapTextureAsset)
	{
		TerrainEntity->SetBaseMap(baseMapTextureAsset->mTexture);
	}
	if (detailMap0TextureAsset)
	{
		detailMap0TextureAsset->mTexture->setWrapMode(QTextureWrapMode(QTextureWrapMode::Repeat));
		TerrainEntity->SetDetailMap0(detailMap0TextureAsset->mTexture);
	}
	if (detailMap1TextureAsset)
	{
		detailMap1TextureAsset->mTexture->setWrapMode(QTextureWrapMode(QTextureWrapMode::Repeat));
		TerrainEntity->SetDetailMap1(detailMap1TextureAsset->mTexture);
	}

	TerrainEntity->SetHeightMap(terrainHeightMapTexture);

	if (MapResourceArchive->IsMechAssault2())
	{
		const std::string microDetailMapTexture0Path = MATerrainEntity["TerrainMicroDetailMap0"];
		const std::string microDetailMapTexture1Path = MATerrainEntity["TerrainMicroDetailMap1"];

		std::shared_ptr<MGFTexture> microDetailMap0TextureAsset = MapResourceArchive->LoadAsset<MGFTexture>(microDetailMapTexture0Path);
		std::shared_ptr<MGFTexture> microDetailMap1TextureAsset = MapResourceArchive->LoadAsset<MGFTexture>(microDetailMapTexture1Path);

		if (microDetailMap0TextureAsset)
		{
			microDetailMap0TextureAsset->mTexture->setWrapMode(QTextureWrapMode(QTextureWrapMode::Repeat));
			TerrainEntity->SetMicroDetailMap0(microDetailMap0TextureAsset->mTexture);
		}
		if (microDetailMap1TextureAsset)
		{
			microDetailMap1TextureAsset->mTexture->setWrapMode(QTextureWrapMode(QTextureWrapMode::Repeat));
			TerrainEntity->SetMicroDetailMap1(microDetailMap1TextureAsset->mTexture);
		}

		TerrainEntity->SetMicroDetailTiling0(MATerrainEntity["TerrainMicroDetailTiling0"]);
		TerrainEntity->SetMicroDetailTiling1(MATerrainEntity["TerrainMicroDetailTiling1"]);
	}
	
	TerrainEntity->BuildTerrain();
}

void MGFMap::OnReadEntity_MATerrainInfo(const MABinaryObject& MATerrainInfo)
{

}

void MGFMap::OnReadEntity_MACorporeal(const MABinaryObject& CorporealEntity)
{
	using namespace Qt3DCore;
	using namespace Qt3DRender;
	using namespace Qt3DExtras;

	QEntity* corporealEntity = new QEntity(MapRoot);

	std::shared_ptr<MGFModel> model = MapResourceArchive->LoadAsset<MGFModel>(std::get<std::string>(CorporealEntity.Attributes.at("Corporal3DObj").Value), false);
	
	if (model.get() && model->mRootNode)
	{
		AssetRefs.push_back(model);

		model->mRootNode->setParent(corporealEntity);

		QVector3D position = CorporealEntity["CorporalPos"];
		float pitch = CorporealEntity["CorporalPitch"];
		float yaw = CorporealEntity["CorporalHeading"];
		float roll = CorporealEntity["CorporalRoll"];

		const auto Wrap360 = [](float in){
			//return (int)in % 360 + (in - (int)in);
			return in < 0.0f ? in + 360.0f : in;
		};

		Qt3DCore::QTransform* transform = new Qt3DCore::QTransform;
		transform->setTranslation(position);
		transform->setRotation(QQuaternion::fromEulerAngles(pitch, yaw, roll));

		corporealEntity->addComponent(transform);
	}
}
