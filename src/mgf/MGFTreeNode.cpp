#include "MGFTreeNode.h"

#include <string>
#include <stack>

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
	InitFileType();
}

MGFTreeNode::~MGFTreeNode()
{
	children.clear();
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
		dir.append('/' + nodeTree.top()->Name().ToStdString());
		nodeTree.pop();
	}

	return dir;
}

void MGFTreeNode::InitFileType()
{
	int pos = name.Last('.') + 1;
	wxString extension = name.SubString(pos, name.Length() - pos).Lower();

	if		(std::strcmp(extension, "mgtext") == 0)		fileType = MGFFileType::Strings;
	else if (std::strcmp(extension, "tif") == 0)		fileType = MGFFileType::Texture;
	else if (std::strcmp(extension, "mgmodel") == 0)	fileType = MGFFileType::Model;
}
