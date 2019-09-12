#pragma once

#include <wx/string.h>

#include <vector>
#include <unordered_map>

class MGFArchive;

enum class MGFFileType : char
{
	None,
	Strings,
	Texture,
	Model
};

class MGFTreeNode
{
public:
	MGFTreeNode(MGFTreeNode* parent,
		const wxString& name,
		unsigned int ID,
		unsigned int fileOffset,
		unsigned int fileLength,
		int timestamp,
		bool isFile,
		MGFArchive& archive);

	inline MGFTreeNode* GetParent() const { return parent; }
	inline MGFTreeNode* GetNthChild(int index) const { return children[index]; }
	inline MGFTreeNode* GetNamedChild(const std::string& name) const { return (mapChildren.find(name) != mapChildren.end()) ? mapChildren.at(name) : nullptr; }
	inline MGFTreeNode* GetNamedSibling(const std::string& name) const { return parent->GetNamedChild(name); }

	MGFTreeNode* GetRelativeNode(const std::string& relativePath) const;

	void AddChild(MGFTreeNode* child);
	inline unsigned int GetChildCount() const { return children.size(); }

	inline const wxString& Name() const { return name; }
	inline const unsigned int FileOffset() const { return fileOffset; }
	inline const unsigned int FileLength() const { return fileLength; }
	inline const int LastModifiedDate() const { return timestamp; }
	inline const MGFFileType FileType() const { return fileType; }
	inline const bool IsFile() const { return isFile; }

	inline const wxString& FileExtension() const { return name.SubString(name.rfind('.') + 1, name.Length()); }
	std::string GetFullPath() const;

private:
	wxString name;
	unsigned int ID;
	unsigned int fileOffset;
	unsigned int fileLength;
	int timestamp;
	MGFFileType fileType;
	bool isFile;

	MGFTreeNode* parent;
	std::vector<MGFTreeNode*> children;
	std::unordered_map<std::string, MGFTreeNode*> mapChildren;

private:
	void InitFileType();

public:
	MGFArchive& archive;
};

