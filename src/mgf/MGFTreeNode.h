#pragma once

#include <wx/string.h>

#include <vector>

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
	~MGFTreeNode();

	inline MGFTreeNode* GetParent() const { return parent; }
	inline MGFTreeNode* GetNthChild(int index) const { return children[index]; }

	inline void AddChild(MGFTreeNode* child) { children.push_back(child); }
	inline unsigned int GetChildCount() const { return children.size(); }

	inline const wxString& Name() const { return name; }
	inline const unsigned int FileOffset() const { return fileOffset; }
	inline const unsigned int FileLength() const { return fileLength; }
	inline const int LastModifiedDate() const { return timestamp; }
	inline const MGFFileType FileType() const { return fileType; }
	inline const bool IsFile() const { return isFile; }

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

private:
	void InitFileType();

public:
	MGFArchive& archive;
};

