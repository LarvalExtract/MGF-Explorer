#include "mgmaterialfactory.h"
#include "mgfresourcemanager.h"
#include "utils/configfile.h"
#include "utils/Contexts.h"

#include <Ogre.h>

#include <functional>
#include <unordered_map>
#include <string_view>
#include <cctype>
#include <algorithm>

using namespace std::placeholders;
static std::unordered_map<std::string_view, std::function<void(Ogre::Material&, const MGMaterialDef&, const MGF::File&)>> MapFunctors = {
    { "base",           std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "complex",        std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "specular_mask",  std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "specular_alpha", std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "detail",         std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "color_shift",    std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "env",            std::bind(MGMaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "animated",       std::bind(MGMaterialFactory::CreateAnimatedMaterial,_1, _2, _3) },
    { "distort",        std::bind(MGMaterialFactory::CreateDistortMaterial, _1, _2, _3) },
    { "solid",          std::bind(MGMaterialFactory::CreateSolidMaterial,   _1, _2, _3) }
};

static std::unordered_map<std::string_view, Ogre::SceneBlendType> BlendTypes = {
    { "additive",   Ogre::SceneBlendType::SBT_ADD },
    { "normal",     Ogre::SceneBlendType::SBT_REPLACE },
    { "alpha",      Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA }
};

static std::unordered_map<std::string_view, Ogre::CullingMode> CullModes = {
    { "true",       Ogre::CullingMode::CULL_NONE },
    { "false",      Ogre::CullingMode::CULL_CLOCKWISE }
};

Ogre::MaterialPtr MGMaterialFactory::Create(const MGMaterialDef &params, const MGF::File &sourceFile)
{
    auto& matMgr = Ogre::MaterialManager::getSingleton();

    // generate a unique material name from the model and material name to avoid clashes
    std::string name(sourceFile.Name().toLatin1());
    name += "::" + params.name;

    auto mat = matMgr.getByName(name, "General");

    if (!mat)
    {
        mat = matMgr.create(name, "General");

        auto technique = mat->getTechnique(0);
        auto pass = technique->getPass(0);

        // set common material parameters
        pass->setDiffuse(params.diffuse);
        pass->setEmissive(params.selfillum);
        pass->setShininess(params.shininess);
        pass->setSpecular(params.specular);
        pass->setLightingEnabled(true);
        pass->setVertexColourTracking(Ogre::TVC_NONE);
        pass->setCullingMode(params.cull);
        pass->setSceneBlending(params.blending);

        // upload specialised parameters for different material types
        if (MapFunctors.find(params.type) != MapFunctors.end())
            MapFunctors[params.type](*mat, params, sourceFile);
    }

    return mat;
}

MGMaterialDef MGMaterialFactory::CreateMaterialDefinition(const MGF::File &materialFile)
{
    std::string buf;
    materialFile.LoadBuffer(buf);

    // Load parameters from MA1 .mat file
    ConfigFile meshCfg(buf);
    auto& vars = meshCfg["material"];

    std::transform(vars["type"].begin(), vars["type"].end(), vars["type"].begin(), [](unsigned char c){ return std::tolower(c); });

    // Fetch common material parameters and remove them from the vars map
    MGMaterialDef params;
    params.name         = vars["name"];                                                                 vars.erase("name");
    params.type         = vars["type"];                                                                 vars.erase("type");
    params.diffuse      = StrToColour(vars["diff_color"]);                                              vars.erase("diff_color");
    params.specular     = StrToColour(vars["spec_color"]);                                              vars.erase("spec_color");
    params.selfillum    = StrToColour(vars["selfillum_color"]);                                         vars.erase("selfillum_color");
    params.ambient      = StrToColour(vars["amb_color"]);                                               vars.erase("amb_color");
    params.shininess    = !vars["shininess"].empty() ? std::stof(vars["shininess"]) : 0.0f;             vars.erase("shininess");
    params.shin_strength = !vars["shin_strength"].empty() ? std::stof(vars["shin_strength"]) : 1.0f;    vars.erase("shin_strength");
    params.opacity      = !vars["opacity"].empty() ? std::stof(vars["opacity"]) : 1.0f;                 vars.erase("opacity");
    params.shading      = vars["shading"];                                                              vars.erase("shading");
    params.cull         = CullModes[vars["two_sided"]];                                                 vars.erase("two_sided");
    params.blending     = BlendTypes[vars["blending"]];                                                 vars.erase("blending");

    // Copy remaining vars over to material special parameters
    for (const auto& var : vars)
        params.specialParams.insert(var);

    if (vars.find("base") != vars.end())
    {
        MGTextureParams texture;
        texture.filename = vars.at("base");
        texture.tiling = vars.at("base_tiling");
        texture.filter = Filter::TFO_BILINEAR;
        texture.bMipmapping = true;
        texture.mipmap_bias = 0.5f;

        params.textures.insert(std::make_pair("basetexture", texture));
    }

    return params;
}

MGMaterialDef MGMaterialFactory::CreateMaterialDefinition(const pugi::xml_node &materialxml)
{
    MGMaterialDef params;
    params.name = materialxml.attribute("name").as_string();
    params.type = &materialxml.name()[9];
    params.diffuse = StrToColour(materialxml.attribute("diffuse").as_string());
    params.specular = StrToColour(materialxml.attribute("spec_color").as_string());
    params.selfillum = StrToColour(materialxml.attribute("selfillum").as_string());
    params.shininess = materialxml.attribute("shininess").as_float(1.0f);
    params.shin_strength = materialxml.attribute("shin_strength").as_float();
    params.opacity = materialxml.attribute("opacity").as_float(1.0f);
    params.cull = CullModes[materialxml.attribute("two_sided").as_string()];
    params.blending = BlendTypes[materialxml.attribute("blending").as_string()];

    for (auto attr = materialxml.attribute("blending").next_attribute(); attr; attr = attr.next_attribute())
        params.specialParams.insert(std::make_pair(attr.name(), attr.value()));

    // Load texture parameters from material node
    for (const auto& texture : materialxml.children())
    {
        MGTextureParams texParams;
        texParams.filename = texture.attribute("filename").as_string();
        texParams.tiling = texture.attribute("tiling").as_string();
        texParams.bMipmapping = texture.attribute("two_sided").as_bool();
        texParams.mipmap_bias = texture.attribute("mipmap_bias").as_float();
        texParams.filter = Filter::TFO_BILINEAR;

        params.textures.insert(std::make_pair(texture.name(), texParams));
    }

    return params;
}

void MGMaterialFactory::CreateBaseMaterial(Ogre::Material& mat, const MGMaterialDef& matParams, const MGF::File& mgmodelFile)
{
    const auto& textureParams = matParams.textures.at("basetexture");
    auto textureItem = mgmodelFile.FindRelativeItem(textureParams.filename.data());

    if (textureItem != nullptr)
    {
        auto technique = mat.getTechnique(0);
        auto pass = technique->getPass(0);
        auto tu = pass->createTextureUnitState();
        tu->setTexture(UploadTexture(textureParams, textureItem, *tu));
    }
}

void MGMaterialFactory::CreateComplexMaterial(Ogre::Material& mat, const MGMaterialDef& material_complex_node, const MGF::File& mgmodelFile)
{
    //CreateBaseMaterial(mat, material_complex_node, mgmodelFile);
}

void MGMaterialFactory::CreateSpecMaskMaterial(Ogre::Material& mat, const MGMaterialDef &material_specular_mask, const MGF::File &mgmodelFile)
{
    //CreateBaseMaterial(material_specular_mask, mgmodelFile);
}

void MGMaterialFactory::CreateAnimatedMaterial(Ogre::Material& mat, const MGMaterialDef &params, const MGF::File &mgmodelFile)
{
    const auto& matParams = params.specialParams;
    const auto& textureParams = params.textures.at("basetexture");

    auto technique = mat.getTechnique(0);
    auto pass = technique->getPass(0);

    auto tu = pass->createTextureUnitState();

    uint32_t animMaxFrameNum = std::stoi(matParams.at("animMaxFrameNum").data());
    int32_t animLoops = std::stoi(matParams.at("animLoops").data());
    //bool animBlendFrames = material_animated.attribute("animBlendFrames").as_bool();
    //bool animPlaying = material_animated.attribute("animPlaying").as_bool();

    for (uint32_t i = 1; i <= animMaxFrameNum; i++)
    {
        QString fileName(textureParams.filename.data());

        QString frameIndex(" ");
        if (i + 1 <= 10)
            frameIndex += '0';
        frameIndex += QString::number(i);

        fileName.insert(fileName.lastIndexOf('.'), frameIndex);

        auto textureFile = mgmodelFile.FindRelativeItem(fileName);

        tu->addFrameTextureName(UploadTexture(textureParams, textureFile, *tu)->getName());
    }

    tu->setCurrentFrame(0);
}

void MGMaterialFactory::CreateSolidMaterial(Ogre::Material &mat, const MGMaterialDef &material_solid, const MGF::File &mgmodelFile)
{
    mat.setLightingEnabled(false);
}

void MGMaterialFactory::CreateDistortMaterial(Ogre::Material &mat, const MGMaterialDef &material_distort, const MGF::File &sourceFile)
{
    const auto& textureParams = material_distort.textures.at("distortiontexture");
    auto textureItem = sourceFile.FindRelativeItem(textureParams.filename.data());

    auto technique = mat.getTechnique(0);
    auto pass = technique->getPass(0);
    auto tu = pass->createTextureUnitState();

    tu->setTexture(UploadTexture(textureParams, textureItem, *tu));
}

Ogre::TexturePtr MGMaterialFactory::UploadTexture(const MGTextureParams& textureParams, const MGF::File* textureFile, Ogre::TextureUnitState& tu)
{
    auto& rm = *Contexts::Get<MGFResourceManager>();

    Ogre::TexturePtr texture = rm.GetTexture(*textureFile).m_Texture;

    tu.setTextureFiltering(textureParams.filter);
    tu.setTextureMipmapBias(textureParams.mipmap_bias);
    //tu.setTextureScroll(0.5f, 0.5f);

    return texture;
}

Ogre::ColourValue MGMaterialFactory::StrToColour(std::string_view str)
{
    if (str.empty())
        return Ogre::ColourValue::White;

    auto c1 = str.find(',');
    auto c2 = str.find(',', c1+1);

    Ogre::ColourValue result;
    result.r = std::stof(str.substr(0, c1).data()) / 255.0f;
    result.g = std::stof(str.substr(c1+1, c2).data()) / 255.0f;
    result.b = std::stof(str.substr(c2+1, str.size()).data()) / 255.0f;
    result.a = 1.0f;

    return result;
}
