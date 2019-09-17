#pragma once

#include "../mgf/MGFArchive.h"

#include "TextureViewerPanel.h"
#include "ModelViewerPanel.h"
#include "StringsViewerPanel.h"

#include "../renderer/VertexArray.h"
#include "../renderer/Shader.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/dataview.h>

#include <memory>

class MyNotebook;

class NotebookPage : public wxPanel
{
public:
	NotebookPage(wxWindow* parent, MGFArchive* mgfFile);

	inline const MGFArchive& GetMGFFile() const { return mgfArchive; }

	void OnItemSelected(wxDataViewEvent& event);

private:
	void ShowPanel(wxWindow* newPanel);
	void LoadTextFileData(const MGFTreeNode& txtFileNode);

private:
	wxBoxSizer* sizer;
	wxDataViewCtrl* treeView;

	TextureViewerPanel* textureViewer;
	ModelViewerPanel* modelViewer;
	StringsViewerPanel* stringsViewer;
	wxTextCtrl* txtFileViewer;
	wxPanel* placeholderPanel;

	MGFArchive& mgfArchive;

	wxDECLARE_EVENT_TABLE();
};

