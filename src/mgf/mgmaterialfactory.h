#ifndef MGMATERIALFACTORY_H
#define MGMATERIALFACTORY_H

#include "mgftreeitem.h"

#include <OgreMaterial.h>
#include <pugixml-1.10/src/pugixml.hpp>

using Params = std::unordered_map<std::string, std::string>;
using Filter = Ogre::TextureFilterOptions;

struct MGTextureParams
{
    std::string filename;
    std::string tiling;
    bool bMipmapping = true;
    float mipmap_bias = 0.0f;
    Filter filter = Filter::TFO_BILINEAR;
};

struct MGMaterialDef
{
    std::string name;
    std::string type;
    Ogre::ColourValue diffuse = Ogre::ColourValue::White;
    Ogre::ColourValue ambient = Ogre::ColourValue::White;
    Ogre::ColourValue specular = Ogre::ColourValue::White;
    Ogre::ColourValue selfillum = Ogre::ColourValue::Black;
    float shininess = 0.0f;
    float shin_strength = 1.0f;
    float opacity = 0.0f;
    std::string shading;
    Ogre::CullingMode cull = Ogre::CullingMode::CULL_CLOCKWISE;
    Ogre::SceneBlendType blending = Ogre::SceneBlendType::SBT_REPLACE;

    Params specialParams;
    std::unordered_map<std::string, MGTextureParams> textures;
};

class MGMaterialFactory
{
public:
    MGMaterialFactory() = delete;
    MGMaterialFactory(const MGMaterialFactory&) = delete;
    MGMaterialFactory(MGMaterialFactory&&) = delete;
    MGMaterialFactory& operator=(const MGMaterialFactory&) = delete;
    MGMaterialFactory& operator=(MGMaterialFactory&&) = delete;

    static Ogre::MaterialPtr Create(const MGMaterialDef&, const MGFTreeItem& sourceFile);

    static MGMaterialDef CreateMaterialDefinition(const MGFTreeItem& materialFile);
    static MGMaterialDef CreateMaterialDefinition(const pugi::xml_node& materialxml);


    static void CreateBaseMaterial(Ogre::Material& mat, const MGMaterialDef& material_base, const MGFTreeItem& mgmodelFile);
    static void CreateComplexMaterial(Ogre::Material& mat, const MGMaterialDef& material_complex, const MGFTreeItem& mgmodelFile);
    static void CreateSpecMaskMaterial(Ogre::Material& mat, const MGMaterialDef& material_specular_mask, const MGFTreeItem& mgmodelFile);
    static void CreateAnimatedMaterial(Ogre::Material& mat, const MGMaterialDef& material_animated, const MGFTreeItem& mgmodelFile);
    static void CreateSolidMaterial(Ogre::Material& mat, const MGMaterialDef& material_solid, const MGFTreeItem& mgmodelFile);
    static void CreateDistortMaterial(Ogre::Material& mat, const MGMaterialDef& material_distort, const MGFTreeItem& sourceFile);

private:
    static Ogre::TexturePtr UploadTexture(const MGTextureParams& textureNode, const MGFTreeItem* textureFile, Ogre::TextureUnitState& tu);
    static Ogre::ColourValue StrToColour(std::string_view str);
};

#endif // MGMATERIALFACTORY_H
