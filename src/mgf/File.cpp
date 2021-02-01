#include "File.h"
#include "Archive.h"

#include <sstream>
#include <functional>

const std::unordered_map<std::string, MGF::FileType> MGF::File::MapExtensionFileType = {
    { ".tif",        MGF::FileType::Tif },
    { ".mgmodel",    MGF::FileType::MgModel },
    { ".mgtext",     MGF::FileType::MgText },
    { ".txt",        MGF::FileType::Text },
    { ".ini",        MGF::FileType::Text },
    { ".cfg",        MGF::FileType::Text },
    { ".mesh",       MGF::FileType::Text },    // temp
    { ".road",       MGF::FileType::Text },    // temp
    { ".node",       MGF::FileType::Node },    // temp
    { ".mat",        MGF::FileType::Text },    // temp
    { ".mgv",        MGF::FileType::Text },
    { "",            MGF::FileType::None }
};

using namespace MGF;

File::File(MGF::File* parent,
          const char* name,
          uint64_t id,
          uint32_t index,
          uint32_t offset,
          uint32_t length,
          int32_t  timestamp,
          bool     isFile,
          MGF::Archive& mgfFile) :
    m_Parent(parent),
    m_Name(name),
    m_GUID(id),
    m_Index(index),
    m_FileType(MGFFileType::None),
    m_FileOffset(offset),
    m_FileLength(length),
    //m_FileDate(timestamp),
    m_IsFile(isFile),
    m_MGFArchive(mgfFile),
    m_TreeViewRow(0)
{
    m_FileDate.setTime_t(timestamp);

    if (parent != nullptr)
    {
        parent->AddChildItem(this);
        m_TreeViewRow = parent->GetChildCount() - 1;
        m_FilePath = parent->FilePath();
        m_FilePath.append(name);
    }
    else
    {
        m_FilePath = m_Name.toLatin1().data();
    }

    if (m_IsFile)
    {
        auto toLower = [](std::string& str)
        {
            for (auto& c : str)
                if (c >= 'A' && c <= 'Z')
                    c += ' ';
        };

        auto ext = m_FilePath.extension().u8string();
        toLower(ext);

        if (MapExtensionFileType.find(ext) != MapExtensionFileType.end())
            m_FileType = MapExtensionFileType.at(ext);
    }
}

const MGF::File *MGF::File::GetNthChild(int index) const
{
    return m_ChildrenArray[index];
}

const MGF::File *MGF::File::GetNamedChild(const QString &name) const
{
    return m_Children[name.toLower()];
}

const MGF::File *MGF::File::GetNamedSibling(const QString &name) const
{
    if (m_Parent == nullptr)
        return nullptr;

    return m_Parent->GetNamedChild(name);
}

void MGF::File::LoadBuffer(std::string &out, unsigned int offset, int length) const
{
    if (length < 0)
        length = m_FileLength - offset;
    
    out.resize(length);
    m_MGFArchive.FileStream().seekg(m_FileOffset + offset, std::ios::beg);
    m_MGFArchive.FileStream().read(out.data(), length);
}

void MGF::File::AddChildItem(const MGF::File *item)
{
    m_ChildrenArray.push_back(item);
    m_Children.insert(item->Name().toLower(), item);
}

size_t MGF::File::GetChildCount() const
{
    return m_Children.size();
}

const MGF::File *MGF::File::FindRelativeItem(const QString &relativePath) const
{
    MGF::File* parent = m_Parent;
    MGF::File* result = nullptr;

    QString& path = const_cast<QString&>(relativePath);

    for (int i = 0; i < path.size(); i++)
        if (path[i] == '/')
            path[i] = '\\';

    std::stringstream ss(relativePath.toLower().toStdString());
    std::string name;
    while (std::getline(ss, name, '\\'))
    {
        if (name == "..")
            parent = parent->GetParent();
        else
        {
            result = const_cast<MGF::File*>(parent->GetNamedChild(name.c_str()));
            parent = result;
        }
    }

    return result;
}

const char* MGF::File::FileTypeString(MGFFileType type)
{
    static const char* strings[6] = {
        "",
        "Texture",
        "Model",
        "Node",
        "Strings",
        "Text"
    };

    int index = static_cast<int>(type);

    return strings[index];
}

void MGF::File::Read(char* buffer, std::uint32_t offset, std::uint32_t length) const
{
    m_MGFArchive.FileStream().seekg(offset + m_FileOffset, std::ios::beg);
    m_MGFArchive.FileStream().read(buffer, length);
}
