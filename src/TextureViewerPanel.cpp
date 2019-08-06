#include "TextureViewerPanel.h"

#include <string>
#include <wx/msgdlg.h>

TextureViewerPanel::TextureViewerPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : 
	wxPanel(parent, id, pos, size, style, name)
{
	wxBoxSizer* textureViewerSizer;
	textureViewerSizer = new wxBoxSizer(wxVERTICAL);

	textureDetailsTable = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(424, 50), 0);
	textureDetailsTable->SetMinSize(wxSize(424, 50));
	textureDetailsTable->SetMaxSize(wxSize(424, 50));

	colWidth = textureDetailsTable->AppendTextColumn(wxT("Width"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colHeight = textureDetailsTable->AppendTextColumn(wxT("Height"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colLayers = textureDetailsTable->AppendTextColumn(wxT("Layers"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colMips = textureDetailsTable->AppendTextColumn(wxT("Mips"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colFrames = textureDetailsTable->AppendTextColumn(wxT("Frames"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colFormat = textureDetailsTable->AppendTextColumn(wxT("Format"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);
	colFlags = textureDetailsTable->AppendTextColumn(wxT("Flags"), wxDATAVIEW_CELL_INERT, 60, static_cast<wxAlignment>(wxALIGN_LEFT), 0);

	wxVector<wxVariant> emptyRow(7);
	textureDetailsTable->AppendItem(emptyRow);

	InitGLCanvas();

	textureViewerSizer->Add(textureDetailsTable, 0, wxALL, 5);

	this->SetSizer(textureViewerSizer);
	this->Layout();
}

TextureViewerPanel::~TextureViewerPanel()
{
}

void TextureViewerPanel::SetCurrentContext(const wxGLContext& oglContext)
{
	this->textureViewport->SetCurrent(oglContext);
}

void TextureViewerPanel::DrawMGFTexture(const MGFTexture& texture)
{
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.Width()), 0, 0);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.Height()), 0, 1);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.depth), 0, 2);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.mips), 0, 3);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.frames), 0, 4);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.format), 0, 5);
	textureDetailsTable->SetTextValue(wxString::Format("%02x", texture.flags), 0, 6);

	textureViewport->SetSize(wxSize(texture.Width(), texture.Height()));

	texture.Bind();

	glViewport(0, 0, texture.Width(), texture.Height());
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	textureViewport->SwapBuffers();
}

void TextureViewerPanel::InitGLCanvas()
{
	wxGLAttributes attribs;
	attribs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	textureViewport = new wxGLCanvas(this, attribs, 15000, wxPoint(5, 60), wxSize(200, 200));
}
