#pragma once

#include "MGF/File.h"
#include "EAssetTypes.h"

#include <memory>

namespace MGF {	namespace Asset {

    using AssetPtr = std::shared_ptr<class AssetBase>;

    class AssetBase
	{
    public:
		AssetBase() = delete;
		AssetBase(const AssetBase&) = delete;
		AssetBase(AssetBase&&) = delete;
		AssetBase& operator=(const AssetBase&) = delete;
		AssetBase& operator=(AssetBase&) = delete;

        static AssetPtr Create(const File& soureFile);
        static EAssetType ToAssetType(const EFileType sourceFileType);

		const File& FileRef;
		const EAssetType AssetType;

    protected:
        AssetBase(const File& sourceFile, const MGF::Asset::EAssetType assetType) : FileRef(sourceFile), AssetType(assetType) {}
	};

} }
