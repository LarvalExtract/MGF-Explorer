#include "MGFTreeNode.h"

#include <string>
#include <stack>
#include <sstream>

MGFTreeNode::MGFTreeNode(MGFTreeNode* parent, const wxString& name,	unsigned int ID, unsigned int fileOffset, unsigned int fileLength, int timestamp, bool isFile, MGFArchive& archive) :
	parent(parent),
	name(name),
	ID(ID),
	fileOffset(fileOffset),
	fileLength(fileLength),
	fileType(MGFFileType::None),
	timestamp(timestamp),
	isFile(isFile),
	archive(archive)
{
	if (isFile)
		InitFileType();
}

MGFTreeNode::~MGFTreeNode()
{
	children.clear();
}

static void to_lower(std::string& str)
{
	for (std::size_t i = 0; i < str.size(); i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
}

MGFTreeNode* MGFTreeNode::GetRelativeNode(const std::string& relativePath) const
{
	std::stringstream ss(relativePath);
	std::string item;

	MGFTreeNode* l_parent = this->parent;
	MGFTreeNode* destination = nullptr;
	while (std::getline(ss, item, '\\'))
	{
		to_lower(item);

		if (item == "")
			continue;

		if (l_parent == nullptr)
			return nullptr;

		else if (item == "..")
		{
			l_parent = l_parent->GetParent();
		}
		else
		{
			destination = l_parent->GetNamedChild(item);
			l_parent = destination;
		}
	}

	return destination;
}

void MGFTreeNode::AddChild(MGFTreeNode* child)
{
	children.push_back(child);
	mapChildren.insert(std::make_pair(child->Name().Lower().ToStdString(), children.back()));
}

std::string MGFTreeNode::GetFullPath() const
{
	std::stack<const MGFTreeNode*> nodeTree;
	std::size_t pathLength = 0;

	const MGFTreeNode* temp = this;
	while (temp->GetParent() != nullptr)
	{
		nodeTree.push(temp);
		pathLength += temp->Name().Length() + 1;
		temp = temp->GetParent();
	}

	std::string dir;
	dir.reserve(pathLength - 1);

	while (nodeTree.size() > 0)
	{
		dir.append('\\' + nodeTree.top()->Name().ToStdString());
		nodeTree.pop();
	}

	return dir;
}

void MGFTreeNode::InitFileType()
{
	if (name == "atlas.mgmodel")
	{
		int a = 4;
	}

	int pos = name.Last('.') + 1;
	wxString extension = name.SubString(pos, name.Length()).Lower();

	if		(std::strcmp(extension.c_str(), "mgtext") == 0)		fileType = MGFFileType::Strings;
	else if (std::strcmp(extension.c_str(), "tif") == 0)		fileType = MGFFileType::Texture;
	else if (std::strcmp(extension.c_str(), "mgmodel") == 0)	fileType = MGFFileType::Model;
}
