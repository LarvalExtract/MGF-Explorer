#include "File.h"
#include "Archive.h"

using namespace MGF;

const std::unordered_map<std::string, EFileType> File::MapExtensionFileType = {
    { ".tif",     EFileType::tif },
    { ".mgmodel", EFileType::mgmodel },
    { ".mgtext",  EFileType::mgtext },
    { ".txt",     EFileType::txt },
    { ".ini",     EFileType::txt },
    { ".cfg",     EFileType::txt },
    { ".mesh",    EFileType::txt },    // temp
    { ".road",    EFileType::txt },    // temp
    { ".node",    EFileType::node },
    { ".mat",     EFileType::txt },    // temp
    { ".mgv",     EFileType::txt },
    { ".wdf",     EFileType::wdf },
    { ".sdf",     EFileType::sdf },
    { ".mtb",     EFileType::mtb },
    { "",         EFileType::Unassigned }
};

File::File(File* parent,
          const char* name,
          uint64_t id,
          uint32_t index,
          uint32_t offset,
          uint32_t length,
          int32_t  timestamp,
          bool     isFile,
          Archive& mgfFile) :
    Parent(parent),
	FilePath(parent ? parent->FilePath / name : name),
    Name(name),
    GUID(id),
    Index(index),
    FileType([&fp = this->FilePath]() -> EFileType
	{
		auto extension = fp.extension().u8string();
        std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
        return MapExtensionFileType.find(extension) != MapExtensionFileType.end() ? MapExtensionFileType.at(extension) : EFileType::Unassigned;
	}()),
    FileOffset(offset),
    FileLength(length),
	FileDate(QDateTime::fromSecsSinceEpoch(timestamp)),
    IsFile(isFile),
    MGFArchive(mgfFile),
    ArchiveVersion(MGFArchive.GetArchiveVersion()),
    TreeViewRow(parent ? parent->GetChildCount() : 0)
{
    if (parent != nullptr)
    {
        parent->AddChildItem(this);
    }
}

File* File::GetNthChild(int index) const
{
    return ChildrenArray[index];
}

File* File::GetNamedChild(const QString &name) const
{
    return Children[name.toLower()];
}

File* File::GetNamedSibling(const QString &name) const
{
    if (Parent == nullptr)
        return nullptr;

    return Parent->GetNamedChild(name);
}

void File::AddChildItem(File *item)
{
    ChildrenArray.push_back(item);
    Children.insert(item->Name.toLower(), item);
}

size_t File::GetChildCount() const
{
    return Children.size();
}

const File* File::FindRelativeItem(const std::filesystem::path &relativePath) const
{
    File* node = nullptr;
	
    std::for_each(relativePath.begin(), relativePath.end(), [parent = Parent, &node](const auto& i) 
    {
        parent = i == ".."
            ? parent->Parent
            : node = parent->GetNamedChild(i.u8string().c_str());
    });

    return node;
}

void File::Read(char* buffer, uint32_t offset, uint32_t length) const
{
    MGFArchive.seekg(offset + FileOffset, std::ios::beg);
    MGFArchive.read(buffer, std::min(length, FileLength - offset));
}