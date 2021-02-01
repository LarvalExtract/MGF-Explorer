#pragma once

#include "MGF/File.h"
#include "EAssetTypes.h"

#include <memory>

namespace MGF {	namespace Asset {

    class AssetBase
	{
    public:
        AssetBase(const File& fileRef, const MGF::Asset::EAssetType assetType) : 
            FileRef(fileRef), AssetType(assetType)
        {
            
        }

        inline const File& GetSourceFile() const { return FileRef; }
        inline const EAssetType GetAssetType() const { return AssetType; }

    protected:
        const File& FileRef;
        const EAssetType AssetType;
	};

    using AssetPtr = std::shared_ptr<AssetBase>;

} }
