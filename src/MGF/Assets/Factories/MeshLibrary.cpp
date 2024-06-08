#include "MeshLibrary.h"
#include "MGF/Deserializer.h"
#include "MGF/Structures/geomVert.h"
#include "MGF/Structures/geomFace.h"
#include "MGF/VersionConstants.h"
#include "Utilities/configfile.h"

#include <string_view>
#include <filesystem>

#include <QGeometry>
#include <QGeometryRenderer>
#include <QBuffer>
#include <QAttribute>

Qt3DRender::QGeometryRenderer* MA::MeshLibrary::CreateMesh(const pugi::xml_node& meshxml, const MGF::File& sourceFile)
{
	const std::string_view type = meshxml.attribute("type").as_string();
	const std::string verticesFilename = sourceFile.Name.toStdString() + '{' + meshxml.attribute("vertices").as_string() + '}';
	const std::string indicesFilename = sourceFile.Name.toStdString() + '{' + meshxml.attribute("indices").as_string() + '}';

    return CreateMesh(
        verticesFilename,
        indicesFilename,
        meshxml.attribute("material").as_string(),
        sourceFile,
        type == "indexedstrip" || type == "INDEXEDSTRIP"
            ? Qt3DRender::QGeometryRenderer::PrimitiveType::TriangleStrip
            : Qt3DRender::QGeometryRenderer::PrimitiveType::Triangles
    );
}

Qt3DRender::QGeometryRenderer* MA::MeshLibrary::CreateMesh(const MGF::File& meshFile, const MGF::File& sourceFile)
{
    ConfigFile MeshConfig(&meshFile);
    ConVariables vars = MeshConfig["mesh"];

    const std::string_view type = vars["type"];
    const std::string_view name = vars["name"];
    const std::string_view verticesFilename = vars["vertices"];
    const std::string_view facesFilename = vars["faces"];
    const std::string_view materialFilename = vars["material"];

    return CreateMesh(
        verticesFilename, 
        facesFilename,
        materialFilename,
        sourceFile,
        type == "indexedstrip" || type == "INDEXEDSTRIP" 
            ? Qt3DRender::QGeometryRenderer::PrimitiveType::TriangleStrip 
            : Qt3DRender::QGeometryRenderer::PrimitiveType::Triangles
    );
}

Qt3DRender::QGeometryRenderer* MA::MeshLibrary::CreateMesh(const std::string_view verticesFilename, const std::string_view indicesFilename, const std::string_view material, const MGF::File& sourceFile, Qt3DRender::QGeometryRenderer::PrimitiveType primitiveType)
{
    const MGF::File* verticesFile = sourceFile.FindRelativeItem(verticesFilename);
    const MGF::File* indicesFile = sourceFile.FindRelativeItem(indicesFilename);

    const uint64_t key =
        static_cast<uint64_t>(verticesFile->FilepathHash) << 32 |
        static_cast<uint64_t>(indicesFile->FilepathHash);

    if (mGeometryMap.contains(key))
    {
        return mGeometryMap.at(key);
    }

    QVector3D minExtents, maxExtents;

    Qt3DCore::QGeometry* geom = new Qt3DCore::QGeometry;
    /*int vertexCount = */LoadVertexBuffer(*verticesFile, *geom, minExtents, maxExtents);
    LoadIndexBuffer(*indicesFile, *geom);

    Qt3DRender::QGeometryRenderer* geomRenderer = new Qt3DRender::QGeometryRenderer;
    geomRenderer->setGeometry(geom);
    geomRenderer->setPrimitiveType(primitiveType);
    //geomRenderer->setVertexCount(vertexCount);
    geomRenderer->setMinPoint(minExtents);
    geomRenderer->setMaxPoint(maxExtents);

    mGeometryMap.insert(std::make_pair(key, geomRenderer));

    geomRenderer->setProperty("material", material.data());

    return geomRenderer;
}

int MA::MeshLibrary::LoadVertexBuffer(const MGF::File& verticesFile, Qt3DCore::QGeometry& geom, QVector3D& minExtents, QVector3D& maxExtents)
{
	Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer;
    vertexBuffer->setAccessType(Qt3DCore::QBuffer::Read);
    vertexBuffer->setUsage(Qt3DCore::QBuffer::StaticDraw);

    uint32_t vtxFlags, vtxCount, vtxSize;
    QByteArray vtxData;

	if (MGF::Deserializer deserializer(verticesFile); verticesFile.ArchiveVersion == MGF::Version::MechAssault2LW)
	{
		const GEOMVERT_MA2 header = deserializer.Deserialize<GEOMVERT_MA2>();
        vtxFlags = header.vers.info.vertexBufferLayoutFlags;
        vtxCount = header.vers.info.vertexCount;
        vtxSize = header.vers.data_size - 8;

        vtxData.resize(vtxSize);
        deserializer.ReadBytes(vtxData.data(), vtxSize, sizeof(header));

        minExtents.setX(header.vers.info.extents.min.x);
        minExtents.setY(header.vers.info.extents.min.y);
        minExtents.setZ(header.vers.info.extents.min.z);
        maxExtents.setX(header.vers.info.extents.max.x);
        maxExtents.setY(header.vers.info.extents.max.y);
        maxExtents.setZ(header.vers.info.extents.max.z);
	}
	else
	{
        const GEOMVERT_MA1 header = deserializer.Deserialize<GEOMVERT_MA1>();
        vtxFlags = header.vers.info.vertexBufferLayoutFlags;
        vtxCount = header.vers.info.vertexCount;
        vtxSize = header.vers.data_size - 8;
        vtxData.resize(vtxSize);
        deserializer.ReadBytes(vtxData.data(), vtxSize, sizeof(header));

        minExtents.setX(header.vers.info.extents.min.x);
        minExtents.setY(header.vers.info.extents.min.y);
        minExtents.setZ(header.vers.info.extents.min.z);
        maxExtents.setX(header.vers.info.extents.max.x);
        maxExtents.setY(header.vers.info.extents.max.y);
        maxExtents.setZ(header.vers.info.extents.max.z);
	}

    vtxData.squeeze();
    vertexBuffer->setData(vtxData);

    SetGeometryAttributesFromFlags(vtxFlags, vtxCount, vtxSize / vtxCount, geom, vertexBuffer);

    return vtxCount;
}

void MA::MeshLibrary::LoadIndexBuffer(const MGF::File& indicesFile, Qt3DCore::QGeometry& geom)
{
    Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer;
    indexBuffer->setAccessType(Qt3DCore::QBuffer::Read);
    indexBuffer->setUsage(Qt3DCore::QBuffer::StaticDraw);

    uint32_t idxCount, idxSize;
    QByteArray idxData;

    if (MGF::Deserializer deserializer(indicesFile); indicesFile.ArchiveVersion == MGF::Version::MechAssault2LW)
    {
        const GEOMFACE_MA2 header = deserializer.Deserialize<GEOMFACE_MA2>();
        idxCount = header.indx.nidx.indexCount;
        idxSize = header.indx.data_size - 8;
        idxData.resize(idxSize);
        deserializer.ReadBytes(idxData.data(), idxSize, sizeof(header));
    }
    else
    {
        const GEOMFACE_MA1 header = deserializer.Deserialize<GEOMFACE_MA1>();
        idxCount = header.indx.nidx.indexCount;
        idxSize = header.indx.data_size - 8;
        idxData.resize(idxSize);
        deserializer.ReadBytes(idxData.data(), idxSize, sizeof(header));
    }

    idxData.squeeze();
    indexBuffer->setData(idxData);

    Qt3DCore::QAttribute* attrib = new Qt3DCore::QAttribute;
    attrib->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    attrib->setVertexBaseType(Qt3DCore::QAttribute::UnsignedShort);
    attrib->setBuffer(indexBuffer);
    attrib->setCount(idxCount);

    geom.addAttribute(attrib);
}

void MA::MeshLibrary::SetGeometryAttributesFromFlags(uint32_t flags, uint32_t count, uint32_t stride, Qt3DCore::QGeometry& geom, Qt3DCore::QBuffer* vertexBuffer)
{
    using namespace Qt3DCore;

    static const QString attrPosition(QAttribute::defaultPositionAttributeName());
    static const QString attrTexCoord(QAttribute::defaultTextureCoordinateAttributeName());
    static const QString attrPackedNormal("packedNormal");

    //int stride = 0;

    switch (flags)
    {
    case 0x00000003:   // 24 bytes
        //stride = 24;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 2, count, 12, stride));
        break;

    case 0x00000101:    // 16 bytes
        //stride = 16;
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrPackedNormal, QAttribute::VertexBaseType::UnsignedInt, 1, 16, stride));
        break;

    case 0x00000301:    // 20 bytes
        //stride = 20;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 16, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrPackedNormal, QAttribute::VertexBaseType::UnsignedInt, 1, 16, stride));
        break;

    case 0x00000309:    // 24 bytes
        //stride = 24;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 20, stride));
        break;

    case 0x00000501:    // 24 bytes
        //stride = 24;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 16, stride));
        break;

    case 0x00000509:
        //stride = 24;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 20, stride));
        break;

    case 0x00001003:    // 32 bytes
        //stride = 32;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 2, count, 24, stride));
        break;

    case 0x00001101:    // 24 bytes
        //stride = 24;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 2, count, 16, stride));
        break;

    case 0x00002003:    // 40 bytes
        //stride = 40;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 2, count, 24, stride));
        break;

    case 0x00002101:    // 32 bytes
        //stride = 32;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Float, 2, count, 16, stride));
        break;

    case 0x00080101:    // 36 bytes
        //stride = 36;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 36, stride));
        break;

    case 0x00080301:    // 40 bytes
       // stride = 40;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 16, stride));
        break;

    case 0x00100301:    // 36 bytes
        //stride = 36;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 32, stride));
        break;

    case 0x00100501:    // 40 bytes
        //stride = 40;
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 32, stride));
        break;

    case 0x00180301:
        geom.addAttribute(new QAttribute(vertexBuffer, attrPosition, QAttribute::VertexBaseType::Float, 3, count, 0, stride));
        geom.addAttribute(new QAttribute(vertexBuffer, attrTexCoord, QAttribute::VertexBaseType::Short, 2, count, 32, stride));
        break;

	default:

		break;
	}
}
