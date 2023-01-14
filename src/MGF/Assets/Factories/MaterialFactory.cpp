#include "MaterialFactory.h"
#include "Utilities/configfile.h"
#include "MGF/AssetManager.h"
#include "MGF/Assets/TextureAsset.h"
#include "MGFExplorerApplication.h"

#include <functional>
#include <unordered_map>
#include <string_view>
#include <cctype>
#include <algorithm>

using namespace MGF::Factories;
using namespace std::placeholders;

static std::unordered_map<std::string_view, std::function<void(Ogre::Material&, const MGF::Asset::Model::Material&, const MGF::File&)>> MapFunctors = {
    { "base",           std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "complex",        std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "specular_mask",  std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "specular_alpha", std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "detail",         std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "color_shift",    std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "env",            std::bind(MaterialFactory::CreateBaseMaterial,    _1, _2, _3) },
    { "animated",       std::bind(MaterialFactory::CreateAnimatedMaterial,_1, _2, _3) },
    { "distort",        std::bind(MaterialFactory::CreateDistortMaterial, _1, _2, _3) },
    { "solid",          std::bind(MaterialFactory::CreateSolidMaterial,   _1, _2, _3) }
};

/*
static std::unordered_map<std::string_view, Ogre::SceneBlendType> BlendTypes = {
    { "additive",   Ogre::SceneBlendType::SBT_ADD },
    { "normal",     Ogre::SceneBlendType::SBT_REPLACE },
    { "alpha",      Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA }
};

static std::unordered_map<std::string_view, Ogre::CullingMode> CullModes = {
    { "true",       Ogre::CullingMode::CULL_NONE },
    { "false",      Ogre::CullingMode::CULL_CLOCKWISE }
};
*/


Ogre::MaterialPtr MaterialFactory::Create(const MGF::Asset::Model::Material& params, const MGF::File &sourceFile)
{
    auto& matMgr = Ogre::MaterialManager::getSingleton();

    // generate a unique material name from the model and material name to avoid clashes
    std::string name(sourceFile.Name.toLatin1());
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

MGF::Asset::Model::Material MaterialFactory::CreateMaterialDefinition(const MGF::File &materialFile)
{
    // Load parameters from MA1 .mat file
    ConfigFile meshCfg(&materialFile);
    auto& vars = meshCfg["material"];

    std::transform(vars["type"].begin(), vars["type"].end(), vars["type"].begin(), tolower);

    const auto ExtractVar = [&vars](const std::string& key) { return vars.find(key) != vars.end() ? vars.extract(key).mapped() : ""; };

    // Fetch common material parameters and remove them from the vars map
    MGF::Asset::Model::Material params;
	params.name          = ExtractVar("name");
	params.type          = ExtractVar("type");
	params.diffuse       = StrToColour(ExtractVar("diff_color"));
	params.specular      = StrToColour(ExtractVar("spec_color"));
	params.selfillum     = StrToColour(ExtractVar("selfillum_color"));
	params.ambient       = StrToColour(ExtractVar("ambient"));
	params.shininess     = !vars["shininess"].empty() ? std::stof(ExtractVar("shininess")) : 0.0f;
    params.shin_strength = !vars["shin_strength"].empty() ? std::stof(ExtractVar("shin_strength")) : 1.0f;
    params.opacity       = !vars["opacity"].empty() ? std::stof(ExtractVar("opacity")) : 1.0f;
    params.shading       = vars.extract("shading").mapped();
    params.cull          = CullModes[ExtractVar("two_sided")];
    params.blending      = BlendTypes[ExtractVar("blending")];

    // Copy remaining vars over to material special parameters
    for (const auto& var : vars)
        params.specialParams.insert(var);

    if (vars.find("base") != vars.end())
    {
        MGF::Asset::Model::TextureParams texture;
        texture.filename = vars.at("base");
        texture.tiling = vars.find("base_tiling") != vars.end() ? vars.at("base_tiling") : vars.at("tiling");
        texture.filter = Ogre::TextureFilterOptions::TFO_BILINEAR;
        texture.bMipmapping = true;
        texture.mipmap_bias = 0.5f;

        params.textures.insert(std::make_pair("basetexture", texture));
    }

    return params;
}

MGF::Asset::Model::Material MaterialFactory::CreateMaterialDefinition(const pugi::xml_node &materialxml)
{
    MGF::Asset::Model::Material params;
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
        MGF::Asset::Model::TextureParams texParams;
        texParams.filename = texture.attribute("filename").as_string();
        texParams.tiling = texture.attribute("tiling").as_string();
        texParams.bMipmapping = texture.attribute("two_sided").as_bool();
        texParams.mipmap_bias = texture.attribute("mipmap_bias").as_float();
        texParams.filter = Ogre::TextureFilterOptions::TFO_BILINEAR;

        params.textures.insert(std::make_pair(texture.name(), texParams));
    }

    return params;
}

void MaterialFactory::CreateBaseMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_base, const MGF::File& mgmodelFile)
{
    const auto& textureParams = material_base.textures.at("basetexture");
    auto textureItem = mgmodelFile.FindRelativeItem(textureParams.filename.data());

    if (textureItem != nullptr)
    {
        auto technique = mat.getTechnique(0);
        auto pass = technique->getPass(0);
        auto tu = pass->createTextureUnitState();
        tu->setTexture(UploadTexture(textureParams, textureItem, *tu));
    }
}

void MaterialFactory::CreateComplexMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_complex, const MGF::File& mgmodelFile)
{
    //CreateBaseMaterial(mat, material_complex_node, mgmodelFile);
}

void MaterialFactory::CreateSpecMaskMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_specular_mask, const MGF::File& mgmodelFile)
{
    //CreateBaseMaterial(material_specular_mask, mgmodelFile);
}

void MaterialFactory::CreateAnimatedMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_animated, const MGF::File& mgmodelFile)
{
    const auto& matParams = material_animated.specialParams;
    const auto& textureParams = material_animated.textures.at("basetexture");

    auto technique = mat.getTechnique(0);
    auto pass = technique->getPass(0);

    auto tu = pass->createTextureUnitState();

    uint32_t animMaxFrameNum = std::stoi(matParams.at("animMaxFrameNum").data());
    int32_t animLoops = std::stoi(matParams.at("animLoops").data());
    //bool animBlendFrames = material_animated.attribute("animBlendFrames").as_bool();
    //bool animPlaying = material_animated.attribute("animPlaying").as_bool();

    for (uint32_t i = 1; i <= animMaxFrameNum; i++)
    {
        std::filesystem::path fileName(textureParams.filename);

        QString frameIndex(" ");
        if (i + 1 <= 10)
            frameIndex += '0';
        frameIndex += QString::number(i);

        fileName.replace_filename(fileName.filename().string() + frameIndex.toStdString());

        auto textureFile = mgmodelFile.FindRelativeItem(fileName);

        tu->addFrameTextureName(UploadTexture(textureParams, textureFile, *tu)->getName());
    }

    tu->setCurrentFrame(0);
}

void MaterialFactory::CreateSolidMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_solid, const MGF::File& mgmodelFile)
{
    mat.setLightingEnabled(false);
}

void MaterialFactory::CreateDistortMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_distort, const MGF::File& sourceFile)
{
    const auto& textureParams = material_distort.textures.at("distortiontexture");
    auto textureItem = sourceFile.FindRelativeItem(textureParams.filename.data());

    auto technique = mat.getTechnique(0);
    auto pass = technique->getPass(0);
    auto tu = pass->createTextureUnitState();

    tu->setTexture(UploadTexture(textureParams, textureItem, *tu));
}

Ogre::TexturePtr MaterialFactory::UploadTexture(const MGF::Asset::Model::TextureParams& textureParams, const MGF::File* textureFile, Ogre::TextureUnitState& tu)
{
    auto& rm = qApp->AssetManager;
 
    Ogre::TexturePtr texture = static_cast<MGF::Asset::TextureAsset*>(rm.Get(*textureFile).get())->OgreTexture;

    tu.setTextureFiltering(textureParams.filter);
    tu.setTextureMipmapBias(textureParams.mipmap_bias);
    //tu.setTextureScroll(0.5f, 0.5f);

    return texture;
}

Ogre::ColourValue MaterialFactory::StrToColour(std::string_view str)
{
    return str.empty()
        ? Ogre::ColourValue::White
        : [str]()
        {
			auto c1 = str.find(',');
			auto c2 = str.find(',', c1 + 1);

            return Ogre::ColourValue(
                std::stof(str.substr(0, c1).data()) / 255.0f,
				std::stof(str.substr(c1 + 1, c2).data()) / 255.0f,
				std::stof(str.substr(c2 + 1, str.size()).data()) / 255.0f,
                1.0f
            );
		}();
}
