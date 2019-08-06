#pragma once

#include <glad/glad.h>

#include "NotebookPage.h"

#include "mgf/MGFArchive.h"
#include "mgf/MGFResourceManager.h"

#include "renderer/Shader.h"
#include "renderer/VertexArray.h"

#include <wx/notebook.h>
#include <wx/glcanvas.h>

#include <string>
#include <vector>
#include <filesystem>

class MyNotebook : public wxNotebook
{
public:
	MyNotebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
	~MyNotebook();

	MGFResourceManager mgfResources;

	void OpenMgfFile(const std::filesystem::path& archiveFilepath);
	void CloseMgfFile(int index);
	void CloseAllMgfFiles();
	bool MgfFileIsOpen(const std::string& archiveName);

	void SelectMgfFile(const std::string& archiveName);

	inline const wxGLContext& GetGLContext() const { return *oglContext; }

private:
	std::unordered_map<std::string, MGFArchive*> mapArchives;

	// Events
private:

	// OpenGL stuff
private:
	bool bGLInitialised;
	wxGLContext* oglContext;

public:
	std::unique_ptr<VertexArray> textureViewVAO;
	std::unique_ptr<ShaderProgram> textureViewShader;

	// Methods
private:
	bool InitOpenGL();
};