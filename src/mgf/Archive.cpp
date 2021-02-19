#include "Archive.h"

#include "MGF/Structures/mgf.h"

#include <exception>

MGF::Archive::Archive(const QString& mgfFilePath) :
    m_FilePath(mgfFilePath)
{
    int lastSeparator = m_FilePath.lastIndexOf('/') + 1;
    int length = m_FilePath.size() - lastSeparator;
    m_FileName = m_FilePath.mid(lastSeparator, length);

    m_FileStream.open(mgfFilePath.toStdString(), std::ios::binary);

    if (!m_FileStream.is_open())
        throw std::runtime_error("Couldn't open " + mgfFilePath.toStdString());

    // read header from MGF file (first 64 bytes)
    MGF_HEADER mgfHeader;
    m_FileStream.seekg(0, std::ios::beg);
    m_FileStream.read(reinterpret_cast<char*>(&mgfHeader), sizeof(mgfHeader));

    // check for valid MGF file
    if (!(mgfHeader.signature[0] == 'm' &&
		  mgfHeader.signature[1] == 'g' &&
		  mgfHeader.signature[2] == 'f' &&
		  mgfHeader.signature[3] == ' '))
    {
        throw std::runtime_error(mgfFilePath.toStdString() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");
    }  

    ArchiveVersion = static_cast<MGF::Version>(mgfHeader.version);
    FileCount = mgfHeader.fileRecordCount;

    std::vector<MGF_FILE_RECORD_MA1> fileRecordsMa1;
    std::vector<MGF_FILE_RECORD_MA2> fileRecordsMa2;

    if (ArchiveVersion == Version::MechAssault2LW)
    {
		fileRecordsMa2.resize(mgfHeader.fileRecordCount);
        m_FileStream.read(reinterpret_cast<char*>(fileRecordsMa2.data()), mgfHeader.fileRecordLength);
    }
    else
    {
		fileRecordsMa1.resize(mgfHeader.fileRecordCount);
        m_FileStream.read(reinterpret_cast<char*>(fileRecordsMa1.data()), mgfHeader.fileRecordLength);
    }

	std::vector<MGF_DIRECTORY> directoryRows(mgfHeader.indexTableCount);
	m_FileStream.read(reinterpret_cast<char*>(directoryRows.data()), mgfHeader.indexTableLength);

    std::vector<char> stringBuffer(mgfHeader.stringsLength);
    m_FileStream.read(stringBuffer.data(), mgfHeader.stringsLength);

	m_TreeItems.reserve(mgfHeader.indexTableCount);
	m_TreeItems.emplace_back(nullptr, &stringBuffer[directoryRows[0].stringOffset], 0, 0, 0, 0, 0, false, *this);

    if (ArchiveVersion == Version::MechAssault2LW)
    {
		for (size_t i = 1; i < directoryRows.size(); i++)
		{
			// item is a file
			if (directoryRows[i].fileIndex != -1)
			{
				m_TreeItems.emplace_back(
					&m_TreeItems[directoryRows[i].parentIndex],
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
				m_TreeItems.emplace_back(
					&m_TreeItems[directoryRows[i].parentIndex],
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
				m_TreeItems.emplace_back(
					&m_TreeItems[directoryRows[i].parentIndex],
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
				m_TreeItems.emplace_back(
					&m_TreeItems[directoryRows[i].parentIndex],
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

    m_FileStream.seekg(0, std::ios::end);
    FileSize = m_FileStream.tellg();
}
