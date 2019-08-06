#pragma once

#include <glad/glad.h>

#include "mgf/MGFTexture.h"

#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/glcanvas.h>

#include "renderer/Shader.h"

class TextureViewerPanel : public wxPanel
{
public:
	TextureViewerPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(851, 615), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
	~TextureViewerPanel();

	void SetCurrentContext(const wxGLContext& oglContext);
	void DrawMGFTexture(const MGFTexture& texture);

private:
	wxDataViewListCtrl* textureDetailsTable;
	wxDataViewColumn* colWidth;
	wxDataViewColumn* colHeight;
	wxDataViewColumn* colLayers;
	wxDataViewColumn* colMips;
	wxDataViewColumn* colFrames;
	wxDataViewColumn* colFormat;
	wxDataViewColumn* colFlags;

	wxGLCanvas* textureViewport;

private:
	void InitGLCanvas();
};

