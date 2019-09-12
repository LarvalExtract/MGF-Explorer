#pragma once

#include "../mgf/MATexture.h"

#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/glcanvas.h>

#include "../renderer/Shader.h"
#include "../renderer/VertexArray.h"

class TextureViewerPanel : public wxPanel
{
public:
	TextureViewerPanel(wxWindow* parent);
	~TextureViewerPanel();

	void DrawMGFTexture(const MATexture& texture);
	void SetGLContextCurrent();

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

	std::shared_ptr<ShaderProgram> textureview_shader;
	static VertexArray* vao;
	static int count;
};

