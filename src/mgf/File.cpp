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

File::File(
    int32_t  index,
    int32_t  parentIndex,
    int32_t  siblingIndex,
    int32_t  childIndex,
    Archive& mgfFile,
    const char* name,
    uint32_t hash,
    uint32_t checksum,
    uint32_t offset,
    uint32_t length,
    int32_t  timestamp,
    bool     isFile)
: _Index(index)
, _ParentIndex(parentIndex)
, _SiblingIndex(siblingIndex)
, _ChildIndex(childIndex)
, MGFArchive(mgfFile)
, FilePath(Parent() ? Parent()->FilePath / name : name)
, Name(name)
, FilepathHash(hash)
, FileChecksum(checksum)
, FileType([&fp = this->FilePath]() -> EFileType
{
	auto extension = fp.extension().u8string();
    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
    return MapExtensionFileType.find(extension) != MapExtensionFileType.end() ? MapExtensionFileType.at(extension) : EFileType::Unassigned;
}())
, FileOffset(offset)
, FileLength(length)
, FileDate(QDateTime::fromSecsSinceEpoch(timestamp))
, IsFile(isFile)
, ArchiveVersion(MGFArchive.ArchiveVersion)
{
    if (Parent())
    {
        const_cast<File*>(Parent())->_ChildCount++;
    }
}

const File* File::Parent() const
{
    return _ParentIndex != -1 ? &MGFArchive.Files[_ParentIndex] : nullptr;
}

const File* File::Sibling() const
{
    return _SiblingIndex != -1 ? &MGFArchive.Files[_SiblingIndex] : nullptr;
}

const File* File::Child() const
{
    return _ChildIndex != -1 ? &MGFArchive.Files[_ChildIndex] : nullptr;
}

const File* File::ChildAt(size_t at) const
{
    return Child() ? Child()->SiblingAt(at) : nullptr;
}

const File* File::SiblingAt(size_t at) const
{
    auto sibling = this;
	for (size_t i = 0; sibling && i < at; i++)
	{
        sibling = sibling->Sibling();
	}
    return sibling;
}

const File* File::FindRelativeItem(const std::filesystem::path &relativePath) const
{
    const File* parent = Parent();
    const File* target = nullptr;
	
    std::for_each(relativePath.begin(), relativePath.end(), [&parent, &target](const auto& i) 
    {
        parent = i == ".."
            ? parent->Parent()
            : target = [parent](QString&& name) -> const File*
            {
				for (auto child = parent->Child(); child; child = child->Sibling())
				{
					if (child->Name.toLower() == name.toLower())
					{
                        return child;
					}
				}
                return nullptr;
            	
			}(QString(i.u8string().c_str()));
    });

    return target;
}

void File::Read(char* buffer, uint32_t offset, uint32_t length) const
{
    MGFArchive.seekg(offset + FileOffset, std::ios::beg);
    MGFArchive.read(buffer, std::min(length, FileLength - offset));
}