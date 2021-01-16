#ifndef MGTEXTURE_H
#define MGTEXTURE_H

#include "File.h"

#include <stdint.h>
#include <OgreTexture.h>

class MGTexture
{
public:
    MGTexture(MGF::File& item, bool loadHeaderOnly = false);

    inline const MGF::File& TreeItem() const { return m_TreeItem; }
    inline uint8_t Version() const { return m_Version; }
    inline uint32_t Width() const { return m_Width; }
    inline uint32_t Height() const { return m_Height; }
    inline uint32_t Flags() const { return m_Flags; }
    inline uint32_t Mips() const { return m_Mips; }
    inline uint32_t Frames() const { return m_Frames; }
    inline uint32_t Depth() const { return m_Depth; }
    inline uint32_t Size() const { return m_Size; }
    inline uint32_t Bits() const { return m_Bits; }

    Ogre::TexturePtr m_Texture;

private:
    MGF::File& m_TreeItem;

    uint8_t m_Version;
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Flags;
    uint32_t m_Mips;
    uint32_t m_Frames;
    uint32_t m_Depth;
    uint32_t m_Size;
    uint32_t m_Bits; 

private:
    void LoadHeader();
    Ogre::TextureType DetermineTextureType();
    Ogre::PixelFormat DeterminePixelFormat();
    void LoadTextureData();
};

#endif // MGTEXTURE_H
