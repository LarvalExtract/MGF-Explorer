#include "Archive.h"
#include "widgets/mgfworkspace.h"

struct MGFFileEntryStruct
{
    unsigned int offset_file_index;
    unsigned int offset_file_guid;
    unsigned int offset_file_length;
    unsigned int offset_file_offset;
    unsigned int offset_file_timestamp;
    unsigned int size_struct;
};

MGF::Archive::Archive(const QString& mgfFilePath) :
    m_FilePath(mgfFilePath),
    m_FileTreeModel(m_TreeItems),
    m_FileTableModel(m_TreeItems)
{
    int lastSeparator = m_FilePath.lastIndexOf('/') + 1;
    int length = m_FilePath.size() - lastSeparator;
    m_FileName = m_FilePath.mid(lastSeparator, length);

    m_FileStream.open(mgfFilePath.toStdString(), std::ios::binary);

    if (!m_FileStream.is_open())
        throw std::runtime_error("Couldn't open " + mgfFilePath.toStdString());

    // read header from MGF file (first 64 bytes)
    constexpr int headerLength = 16;
    uint32_t header[headerLength];
    m_FileStream.read(reinterpret_cast<char*>(&header), 64);

    constexpr uint32_t MGF_SIGNATURE = 0x2066676D; // "mgf "

    // check for valid MGF file
    if (header[0] != MGF_SIGNATURE)
        throw std::runtime_error(mgfFilePath.toStdString() + " is either compressed or not a valid MechAssault 1 or MechAssault 2: Lone Wolf MGF file");

    // copy header values
    m_ArchiveVersion = static_cast<uint8_t>(header[1]);
    m_FileEntryCount = header[3];
    m_FileEntryLength = header[4];
    m_FileEntryOffset = header[5];
    m_IndexEntryCount = header[6];
    m_IndexEntryLength = header[7];
    m_IndexEntryOffset = header[8];
    m_StringListLength = header[9];
    m_StringListOffset = header[10];


    MGFFileEntryStruct descriptor;

    // game version determined by header value: 4 = MA2, 2 = MA1
    // file entry data is different for each game
    if (m_ArchiveVersion == 4)
    {
        descriptor.offset_file_index = 0;
        descriptor.offset_file_guid = 4;
        descriptor.offset_file_length = 12;
        descriptor.offset_file_timestamp = 20;
        descriptor.offset_file_offset = 24;
        descriptor.size_struct = 32;
    }
    else if (m_ArchiveVersion == 2)
    {
        descriptor.offset_file_index = 4;
        descriptor.offset_file_guid = 12;
        descriptor.offset_file_length = 20;
        descriptor.offset_file_timestamp = 8;
        descriptor.offset_file_offset = 24;
        descriptor.size_struct = 28;
    }
    else
    {
        throw std::runtime_error(mgfFilePath.toStdString() + " is not a supported version (version: " + std::to_string(m_ArchiveVersion));
    }

    LoadMechAssaultFileData(descriptor);

    m_FileStream.seekg(0, std::ios::end);
    m_FileSize = m_FileStream.tellg();
}

struct MGFIndexEntry
{
    int32_t unknown1;       // 0 for files, non-zero for folders
    int32_t parentIndex;    // identifies the folder this item belongs to
    int32_t folderIndex;    // index if the item is a folder, -1 if item is a file (this fills the index gaps left in the file entry table)
    int32_t unknown3;
    int32_t stringOffset;
    int32_t isFile;
};

void MGF::Archive::LoadMechAssaultFileData(const MGFFileEntryStruct& descriptor)
{
    // copy file entry bytes to buffer
    std::vector<char> fileEntryBuffer(m_FileEntryLength);
    m_FileStream.seekg(m_FileEntryOffset);
    m_FileStream.read(fileEntryBuffer.data(), m_FileEntryLength);

    // copy index entry bytes to buffer
    std::vector<MGFIndexEntry> indexEntries(m_IndexEntryCount);
    m_FileStream.seekg(m_IndexEntryOffset);
    m_FileStream.read(reinterpret_cast<char*>(indexEntries.data()), m_IndexEntryLength);

    // copy file/folder string bytes to buffer
    std::vector<char> stringBuffer(m_StringListLength);
    m_FileStream.seekg(m_StringListOffset);
    m_FileStream.read(stringBuffer.data(), m_StringListLength);

    m_TreeItems.reserve(m_IndexEntryCount);
    m_TreeItems.emplace_back(nullptr, &stringBuffer[indexEntries[0].stringOffset], 0, 0, 0, 0, 0, false, *this);

    for (size_t i = 1, j = 0; i < indexEntries.size(); i++)
    {
        // item is a file
        if (indexEntries[i].isFile > -1)
        {
            int32_t index   =  *reinterpret_cast<int32_t*>(&fileEntryBuffer[descriptor.offset_file_index + j * descriptor.size_struct]);
            uint64_t guid   = *reinterpret_cast<uint64_t*>(&fileEntryBuffer[descriptor.offset_file_guid + j * descriptor.size_struct]);
            uint32_t offset = *reinterpret_cast<uint32_t*>(&fileEntryBuffer[descriptor.offset_file_offset + j * descriptor.size_struct]);
            uint32_t length = *reinterpret_cast<uint32_t*>(&fileEntryBuffer[descriptor.offset_file_length + j * descriptor.size_struct]);
            int32_t time    =  *reinterpret_cast<int32_t*>(&fileEntryBuffer[descriptor.offset_file_timestamp + j * descriptor.size_struct]);

            m_TreeItems.emplace_back(
                    &m_TreeItems[indexEntries[i].parentIndex],
                    &stringBuffer[indexEntries[i].stringOffset],
                    guid,
                    index,
                    offset,
                    length,
                    time,
                    true,
                    *this);
            j++;

            // count number of textures
            if (m_TreeItems.back().FileType() == MGFFileType::Texture)
                m_NumTextures++;
        }

        // item is a folder
        else
        {
            m_TreeItems.emplace_back(
                    &m_TreeItems[indexEntries[i].parentIndex],
                    &stringBuffer[indexEntries[i].stringOffset],
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
