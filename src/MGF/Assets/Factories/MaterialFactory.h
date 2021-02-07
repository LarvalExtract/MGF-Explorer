#ifndef MGMATERIALFACTORY_H
#define MGMATERIALFACTORY_H

#include "MGF/Assets/ModelAsset.h"

#include <OgreMaterial.h>

#include <pugixml-1.10/src/pugixml.hpp>

namespace MGF { namespace Factories {

	class MaterialFactory
	{
	public:
		MaterialFactory() = delete;
		MaterialFactory(const MaterialFactory&) = delete;
		MaterialFactory(MaterialFactory&&) = delete;
		MaterialFactory& operator=(const MaterialFactory&) = delete;
		MaterialFactory& operator=(MaterialFactory&&) = delete;

		static Ogre::MaterialPtr Create(const MGF::Asset::Model::Material&, const MGF::File& sourceFile);

		static MGF::Asset::Model::Material CreateMaterialDefinition(const MGF::File& materialFile);
		static MGF::Asset::Model::Material CreateMaterialDefinition(const pugi::xml_node& materialxml);


		static void CreateBaseMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_base, const MGF::File& mgmodelFile);
		static void CreateComplexMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_complex, const MGF::File& mgmodelFile);
		static void CreateSpecMaskMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_specular_mask, const MGF::File& mgmodelFile);
		static void CreateAnimatedMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_animated, const MGF::File& mgmodelFile);
		static void CreateSolidMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_solid, const MGF::File& mgmodelFile);
		static void CreateDistortMaterial(Ogre::Material& mat, const MGF::Asset::Model::Material& material_distort, const MGF::File& sourceFile);

	private:
		static Ogre::TexturePtr UploadTexture(const MGF::Asset::Model::TextureParams& textureNode, const MGF::File* textureFile, Ogre::TextureUnitState& tu);
		static Ogre::ColourValue StrToColour(std::string_view str);
	};

}}

#endif // MGMATERIALFACTORY_H
