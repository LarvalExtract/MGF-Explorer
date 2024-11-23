#include "MGFFile.h"
#include "MGFArchive.h"

MGFFile::MGFFile(
    MGFArchive& mgfArchive,
    const char* name,
    uint32_t hash,
    uint32_t checksum,
    uint32_t offset,
    uint32_t length,
    int32_t  timestamp,
    bool     isFile)
: m_FilePath(name)
, MgfArchive(mgfArchive)
, Name(name)
, FilepathHash(hash)
, FileChecksum(checksum)
, FileOffset(offset)
, FileLength(length)
, FileDate(QDateTime::fromSecsSinceEpoch(timestamp))
, IsFile(isFile)
{
    
}

auto MGFFile::FilePath() const -> const std::filesystem::path&
{
    return m_FilePath;
}

const MGFFile* MGFFile::Parent() const
{
    return m_Parent;
}

const MGFFile* MGFFile::ChildAt(size_t at) const
{
    return m_Children.at(at);
}

const MGFFile* MGFFile::SiblingAt(size_t at) const
{
    return Parent()->ChildAt(at);
}

int MGFFile::Row() const
{
    return m_Row;
}

size_t MGFFile::GetChildCount() const
{
    return m_Children.size();
}

void MGFFile::AddChild(MGFFile* child)
{
    child->m_Parent = this;
    child->m_Row = m_Children.size();
    child->m_FilePath = this->m_FilePath / child->m_FilePath;
    m_Children.push_back(child);
}

auto MGFFile::Children() const -> const std::vector<const MGFFile*>&
{
    return m_Children;
}

const MGFFile* MGFFile::FindRelativeItem(const std::filesystem::path &relativePath) const
{
    const MGFFile* parent = Parent() ? Parent() : MgfArchive.Root();
    const MGFFile* target = nullptr;
	
    std::for_each(relativePath.begin(), relativePath.end(), [&parent, &target](const auto& i) 
    {
        parent = i == ".."
            ? parent->Parent()
            : target = [parent](QString&& name) -> const MGFFile*
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

QDebug& operator<<(QDebug& debug, const MGFFile& mgfFile)
{
    debug << '[' << mgfFile.MgfArchive.Path.filename().u8string() << ':' << mgfFile.FilePath().u8string() << ']';
    return debug;
}
