#include "Archive.h"

#include "MGF/Structures/mgf.h"

MGF::Archive::Archive(const std::filesystem::path& mgfFilePath) :
    std::ifstream(mgfFilePath, std::ios::binary),
	Path(mgfFilePath)
{
    if (!is_open())
        throw std::runtime_error("Couldn't open " + Path.u8string());

    // read header from MGF file (first 64 bytes)
    MGF_HEADER mgfHeader;
    seekg(0, std::ios::beg);
    read(reinterpret_cast<char*>(&mgfHeader), sizeof(mgfHeader));

    // check for valid MGF file
    if (!(mgfHeader.signature[0] == 'm' &&
		  mgfHeader.signature[1] == 'g' &&
		  mgfHeader.signature[2] == 'f' &&
		  mgfHeader.signature[3] == ' '))
    {
        throw std::runtime_error(Path.u8string() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");
    }  

    ArchiveVersion = static_cast<MGF::Version>(mgfHeader.version);
    FileCount = mgfHeader.fileRecordCount;

    std::vector<MGF_FILE_RECORD_MA1> fileRecordsMa1;
    std::vector<MGF_FILE_RECORD_MA2> fileRecordsMa2;

    if (ArchiveVersion == Version::MechAssault2LW)
    {
		fileRecordsMa2.resize(mgfHeader.fileRecordCount);
		seekg(mgfHeader.fileRecordOffset);
        read(reinterpret_cast<char*>(fileRecordsMa2.data()), mgfHeader.fileRecordLength);
    }
    else
    {
		fileRecordsMa1.resize(mgfHeader.fileRecordCount);
		seekg(mgfHeader.fileRecordOffset);
        read(reinterpret_cast<char*>(fileRecordsMa1.data()), mgfHeader.fileRecordLength);
    }

	std::vector<MGF_DIRECTORY> directoryRows(mgfHeader.indexTableCount);
	seekg(mgfHeader.indexTableOffset);
	read(reinterpret_cast<char*>(directoryRows.data()), mgfHeader.indexTableLength);

	std::vector<char> stringBuffer(mgfHeader.stringsLength);
	seekg(mgfHeader.stringsOffset);
    read(stringBuffer.data(), mgfHeader.stringsLength);

	Files.reserve(mgfHeader.indexTableCount);
	Files.emplace_back(nullptr, &stringBuffer[directoryRows[0].stringOffset], 0, 0, 0, 0, 0, false, *this);

    if (ArchiveVersion == Version::MechAssault2LW)
    {
		for (size_t i = 1; i < directoryRows.size(); i++)
		{
			// item is a file
			if (directoryRows[i].fileIndex != -1)
			{
				Files.emplace_back(
					&Files[directoryRows[i].parentIndex],
					&stringBuffer[directoryRows[i].stringOffset],
					fileRecordsMa2[directoryRows[i].fileIndex].guid,
                    fileRecordsMa2[directoryRows[i].fileIndex].index,
                    fileRecordsMa2[directoryRows[i].fileIndex].fileOffset,
                    fileRecordsMa2[directoryRows[i].fileIndex].fileLength,
                    fileRecordsMa2[directoryRows[i].fileIndex].timestamp,
					true,
					*this);
			}

			// item is a folder
			else
			{
				Files.emplace_back(
					&Files[directoryRows[i].parentIndex],
					&stringBuffer[directoryRows[i].stringOffset],
					0,
					0, // this should be an index
					0,
					0,
					0,
					false,
					*this);
			}
		}
    }
    else
    {
		for (size_t i = 1; i < directoryRows.size(); i++)
		{
			// item is a file
			if (directoryRows[i].fileIndex != -1)
			{
				Files.emplace_back(
					&Files[directoryRows[i].parentIndex],
					&stringBuffer[directoryRows[i].stringOffset],
					fileRecordsMa1[directoryRows[i].fileIndex].guid,
					fileRecordsMa1[directoryRows[i].fileIndex].index,
					fileRecordsMa1[directoryRows[i].fileIndex].fileOffset,
					fileRecordsMa1[directoryRows[i].fileIndex].fileLength,
					fileRecordsMa1[directoryRows[i].fileIndex].timestamp,
					true,
					*this);
			}

			// item is a folder
			else
			{
				Files.emplace_back(
					&Files[directoryRows[i].parentIndex],
					&stringBuffer[directoryRows[i].stringOffset],
					0,
					0, // this should be an index
					0,
					0,
					0,
					false,
					*this);
			}
		}
    }

    seekg(0, std::ios::end);
    FileSize = tellg();
}
