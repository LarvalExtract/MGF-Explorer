#pragma once

#include "File.h"

namespace MGF {	namespace Asset {

    class AssetBase
	{
    public:
        AssetBase(const File& fileRef, const MGFFileType expectedFileType) : 
            FileRef(fileRef)
        {
            if (FileRef.FileType() != expectedFileType)
            {
                throw std::runtime_error("Tried to create MGF asset with incorrect file type");
            }
        }
    protected:
        const File& FileRef;
	};

} }
