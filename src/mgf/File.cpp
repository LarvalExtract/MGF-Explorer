#include "File.h"
#include "Archive.h"

#include <ranges>
#include <unordered_map>

using namespace MGF;

static const std::unordered_map<std::string_view, EFileType> MapExtensionFileType = {
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
    Archive& mgfFile,
    const char* name,
    uint32_t hash,
    uint32_t checksum,
    uint32_t offset,
    uint32_t length,
    int32_t  timestamp,
    bool     isFile)
: m_FilePath(name)
, MGFArchive(mgfFile)
, Name(name)
, FilepathHash(hash)
, FileChecksum(checksum)
, FileType(m_FilePath.has_extension() ? GetEFileTypeFromExtension(m_FilePath.extension()) : EFileType::Unassigned)
, FileOffset(offset)
, FileLength(length)
, FileDate(QDateTime::fromSecsSinceEpoch(timestamp))
, IsFile(isFile)
, ArchiveVersion(MGFArchive.ArchiveVersion)
{
    
}

auto File::FilePath() const -> const std::filesystem::path&
{
    return m_FilePath;
}

const File* File::Parent() const
{
    return m_Parent;
}

const File* File::ChildAt(size_t at) const
{
    return m_Children.at(at);
}

const File* File::SiblingAt(size_t at) const
{
    return Parent()->ChildAt(at);
}

int File::Row() const
{
    return m_Row;
}

size_t File::GetChildCount() const
{
    return m_Children.size();
}

void File::AddChild(File* child)
{
    child->m_Parent = this;
    child->m_Row = m_Children.size();
    child->m_FilePath = this->m_FilePath / child->m_FilePath;
    m_Children.push_back(child);
}

auto File::Children() const -> const std::vector<const File*>&
{
    return m_Children;
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
				for (const auto child : parent->m_Children)
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

EFileType File::GetEFileTypeFromExtension(const std::filesystem::path& extension)
{
    auto ext = extension.string();
    for (char& c : ext) { c = std::tolower(c); }
    return MapExtensionFileType.contains(ext) ? MapExtensionFileType.at(ext) : EFileType::Unassigned;
}

QDebug& MGF::operator<<(QDebug& debug, const MGF::File& mgfFile)
{
    debug << '[' << mgfFile.MGFArchive.Path.filename().u8string() << ':' << mgfFile.FilePath().u8string() << ']';
    return debug;
}
