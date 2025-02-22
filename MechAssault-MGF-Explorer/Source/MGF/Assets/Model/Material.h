#pragma once

#include <string>

namespace MGF {	namespace Asset { namespace Model {

	struct TextureParams
	{
		std::string filename;
		std::string tiling;
		bool bMipmapping = true;
		float mipmap_bias = 0.0f;
	};

	struct Material
	{
		std::string name;
		std::string type;
		/*
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

		std::unordered_map<std::string, std::string> specialParams;
		std::unordered_map<std::string, TextureParams> textures;
		*/
	};

} } }