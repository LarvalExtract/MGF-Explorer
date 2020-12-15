#include "mgftreeitem.h"
#include "mgfarchive.h"

#include <sstream>
#include <functional>

const std::unordered_map<std::string, MGFFileType> MGFTreeItem::MapExtensionFileType = {
    { ".tif",        MGFFileType::Texture },
    { ".mgmodel",    MGFFileType::Model },
    { ".mgtext",     MGFFileType::Strings },
    { ".txt",        MGFFileType::Text },
    { ".ini",        MGFFileType::Text },
    { ".cfg",        MGFFileType::Text },
    { ".mesh",       MGFFileType::Text },    // temp
    { ".road",       MGFFileType::Text },    // temp
    { ".node",       MGFFileType::Node },    // temp
    { ".mat",        MGFFileType::Text },    // temp
    { ".mgv",        MGFFileType::Text },
    { "",           MGFFileType::None }
};

MGFTreeItem::MGFTreeItem(MGFTreeItem* parent,
                         const char* name,
                         uint64_t id,
                         uint32_t index,
                         uint32_t offset,
                         uint32_t length,
                         int32_t  timestamp,
                         bool     isFile,
                         MGFArchive& mgfFile) :
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

const MGFTreeItem *MGFTreeItem::GetNthChild(int index) const
{
    return m_ChildrenArray[index];
}

const MGFTreeItem *MGFTreeItem::GetNamedChild(const QString &name) const
{
    return m_Children[name.toLower()];
}

const MGFTreeItem *MGFTreeItem::GetNamedSibling(const QString &name) const
{
    if (m_Parent == nullptr)
        return nullptr;

    return m_Parent->GetNamedChild(name);
}

void MGFTreeItem::LoadBuffer(std::string &out, unsigned int offset, int length) const
{
    if (length < 0)
        length = m_FileLength - offset;
    
    out.resize(length);
    m_MGFArchive.FileStream().seekg(m_FileOffset + offset, std::ios::beg);
    m_MGFArchive.FileStream().read(out.data(), length);
}

void MGFTreeItem::AddChildItem(const MGFTreeItem *item)
{
    m_ChildrenArray.push_back(item);
    m_Children.insert(item->Name().toLower(), item);
}

size_t MGFTreeItem::GetChildCount() const
{
    return m_Children.size();
}

const MGFTreeItem *MGFTreeItem::FindRelativeItem(const QString &relativePath) const
{
    MGFTreeItem* parent = m_Parent;
    MGFTreeItem* result = nullptr;

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
            result = const_cast<MGFTreeItem*>(parent->GetNamedChild(name.c_str()));
            parent = result;
        }
    }

    return result;
}

const char* MGFTreeItem::FileTypeString(MGFFileType type)
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

void MGFTreeItem::Read(std::vector<char>& buffer, std::uint32_t offset, std::uint32_t length) const
{
    offset += m_FileOffset;
    length = std::min<uint32_t>(m_FileLength - offset, length);

    buffer.resize(length);
    m_MGFArchive.FileStream().seekg(offset, std::ios::beg);
    m_MGFArchive.FileStream().read(buffer.data(), length);
}
