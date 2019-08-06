#pragma once

#include "mgf/MGFArchive.h"
#include "TextureViewerPanel.h"

#include "renderer/VertexArray.h"
#include "renderer/Shader.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/dataview.h>

#include <memory>

class MyNotebook;

class NotebookPage : public wxPanel
{
public:
	NotebookPage(MyNotebook* notebook, MGFArchive* mgfFile);
	~NotebookPage();

	inline const MGFArchive& GetMGFFile() const { return mgfArchive; }

	void OnItemSelected(wxDataViewEvent& event);

private:
	void RenderTexture(const MGFTexture& texture, const MyNotebook& parent);

private:
	wxBoxSizer* sizer;
	wxDataViewCtrl* treeView;
	wxPanel* dataView;

	TextureViewerPanel* textureViewer;

	MGFArchive& mgfArchive;

	wxDECLARE_EVENT_TABLE();
};

