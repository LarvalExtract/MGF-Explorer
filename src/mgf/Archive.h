#pragma once

#include "File.h"
#include "VersionConstants.h"

#include <fstream>

namespace MGF {

    class Archive : protected std::ifstream
    {
        friend class File;

    public:
        Archive(const std::filesystem::path& mgfFilePath);

    public:
        inline auto GetFileName() const { return Path.filename(); }
        inline auto GetFilePath() const { return Path; }
        inline auto GetFileSize() const { return FileSize; }
        inline auto GetFileCount() const { return FileCount; }
        inline auto GetArchiveVersion() const { return ArchiveVersion; }

        const auto& GetFileList() const { return Files; }

    private:
        std::filesystem::path Path;
        MGF::Version ArchiveVersion;
        uint32_t FileSize;
		uint32_t FileCount;

        std::vector<MGF::File> Files;
    };

}
