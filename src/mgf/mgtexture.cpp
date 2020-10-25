#include "mgtexture.h"
#include "mgfarchive.h"

#include <OgreTextureManager.h>

MGTexture::MGTexture(MGFTreeItem &item, bool loadHeaderOnly) :
    m_TreeItem(item)
{
    LoadHeader();

    if (!loadHeaderOnly)
        LoadTextureData();
}

void MGTexture::LoadHeader()
{
    std::vector<char> header;
    m_TreeItem.Read(header, 0, 149);

    const int version_offset = 56;
    const int flags_offset = 65;
    const int width_offset = 77;
    const int height_offset = 89;
    const int mips_offset = 101;
    const int size_offset = 113;
    const int frames_offset = 125;  // only present in MA2 textures
    const int depth_offset = 137;   // only present in MA2 textures
    int bits_offset = 149;          // different for MA1 and MA2 textures

    m_Version = header[version_offset];
    m_Flags = *reinterpret_cast<uint32_t*>(&header[flags_offset]);
    m_Width = *reinterpret_cast<uint32_t*>(&header[width_offset]);
    m_Height = *reinterpret_cast<uint32_t*>(&header[height_offset]);
    m_Mips = *reinterpret_cast<uint32_t*>(&header[mips_offset]);
    m_Size = *reinterpret_cast<uint32_t*>(&header[size_offset]);

    // MA2 texture, read frames and depth
    if (m_Version == 4)
    {
        m_Frames = 1;//*reinterpret_cast<uint32_t*>(&header[frames_offset]);
        m_Depth = 1;//*reinterpret_cast<uint32_t*>(&header[depth_offset]);
    }
    // MA1 texture, change bits offset and dont read frames/depth
    else
    {
        bits_offset = 125;
        m_Frames = 1;
        m_Depth = 1;
    }

    m_Bits = *reinterpret_cast<uint32_t*>(&header[bits_offset - 4]) - 8;
}

Ogre::TextureType MGTexture::DetermineTextureType()
{
    if (m_Depth > 1)
    {
        if (m_Depth == 6)
            return Ogre::TextureType::TEX_TYPE_CUBE_MAP;
        else
            return Ogre::TextureType::TEX_TYPE_3D;
    }

    if (m_Frames > 1)
        return Ogre::TextureType::TEX_TYPE_2D_ARRAY;

    return Ogre::TextureType::TEX_TYPE_2D;
}

Ogre::PixelFormat MGTexture::DeterminePixelFormat()
{
    uint8_t uncompressed_format_bits = m_Flags & 0x0F;
    uint8_t compressed_format_bits  = (m_Flags >> 8) & 0x0F;

    switch (compressed_format_bits)
    {
    case 1: return Ogre::PixelFormat::PF_DXT1;
    case 3: return Ogre::PixelFormat::PF_DXT3;
    case 5: return Ogre::PixelFormat::PF_DXT5;
    }

    switch (uncompressed_format_bits)
    {
    case 0: return Ogre::PixelFormat::PF_A8R8G8B8;
    case 1: return Ogre::PixelFormat::PF_R5G6B5;
    case 3: return Ogre::PixelFormat::PF_A4R4G4B4;
    case 5: return Ogre::PixelFormat::PF_A8;
    case 7: return Ogre::PixelFormat::PF_L8;
    }
}

void MGTexture::LoadTextureData()
{
    auto& textureManager = Ogre::TextureManager::getSingleton();

    m_Texture = textureManager.getByName(std::to_string(m_TreeItem.GUID()));

    if (!m_Texture)
    {
        auto type = DetermineTextureType();
        auto pf = DeterminePixelFormat();
        Ogre::String name = m_TreeItem.Name().toLatin1().data();

        size_t actualWidth = (m_Width % 16 == 0) ? m_Width : m_Width + (16 - (m_Width % 16));
        auto size = Ogre::Image::calculateSize(m_Mips - 1, m_Frames, actualWidth, m_Height, m_Depth, pf);

        std::vector<char> pixelData;
        m_TreeItem.Read(pixelData, m_Version == 4 ? 149 : 125, size); // add 16??

        Ogre::DataStreamPtr stream;
        stream.reset(new Ogre::MemoryDataStream(pixelData.data(), pixelData.size()));

        Ogre::Image img;
        img.loadRawData(stream,
                        actualWidth,
                        m_Height,
                        m_Depth,
                        pf,
                        m_Frames,
                        m_Mips - 1);

        m_Texture = textureManager.loadImage(
                    std::to_string(m_TreeItem.GUID()),
                    "General",
                    img,
                    type
                    );
    }
}
