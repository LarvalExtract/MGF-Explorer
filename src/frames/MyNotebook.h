#pragma once

#include "NotebookPage.h"

#include "../mgf/MGFArchive.h"

#include "../renderer/Shader.h"
#include "../renderer/VertexArray.h"

#include <wx/notebook.h>

#include <string>
#include <vector>
#include <filesystem>

class MyNotebook : public wxNotebook
{
public:
	MyNotebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
	~MyNotebook();

	void OpenMgfFile(const std::filesystem::path& archiveFilepath);
	void CloseMgfFile(int index);
	void CloseAllMgfFiles();
	bool MgfFileIsOpen(const std::string& archiveName);

	void SelectMgfFile(const std::string& archiveName);

private:
	std::unordered_map<std::string, MGFArchive*> mapArchives;
};