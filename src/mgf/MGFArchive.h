#pragma once

#include "MGFTreeNode.h"

#include <wx/string.h>
#include <wx/dataview.h>

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unordered_map>

class MGFArchive : public wxDataViewModel
{
public:
	MGFArchive(const std::filesystem::path& filepath);
	~MGFArchive();

	inline const wxString& FileName() const { return filename; }
	inline const wxString& ArchiveName() const { return filename.Mid(0, filename.Last('.')); }
	inline unsigned int Size() const { return size; }
	inline std::ifstream& FileStream() { return fileStream; }

	inline unsigned int FileCount() const { return fileEntryCount; }

	inline const MGFTreeNode* RootNode() const { return &treeNodes[0]; }

public:
	virtual wxString GetColumnType(unsigned int col) const override;
	virtual unsigned int GetColumnCount() const override;
	virtual void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const override;
	virtual bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) override;
	virtual bool IsEnabled(const wxDataViewItem& WXUNUSED(item), unsigned int WXUNUSED(col)) const override;
	virtual wxDataViewItem GetParent(const wxDataViewItem& item) const override;
	virtual bool IsContainer(const wxDataViewItem& item) const override;
	virtual unsigned int GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const override;

private:
	std::ifstream fileStream;

	wxString filename;
	unsigned int size;

	unsigned int fileEntryCount;
	unsigned int fileEntryLength;
	unsigned int fileEntryOffset;
	unsigned int indexEntryCount;
	unsigned int indexEntryOffset;
	unsigned int indexEntryLength;
	unsigned int stringsLength;
	unsigned int stringsOffset;

	std::vector<MGFTreeNode> treeNodes;

private:
	bool Validate();
	void InitTreeModel();

	wxIcon fileIcon;
	wxIcon folderIcon;
};

