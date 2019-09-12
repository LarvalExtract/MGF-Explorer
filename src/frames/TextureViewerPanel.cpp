#include "TextureViewerPanel.h"

#include "../application.h"

int TextureViewerPanel::count = 0;
VertexArray* TextureViewerPanel::vao = nullptr;

TextureViewerPanel::TextureViewerPanel(wxWindow* parent) : 
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME)
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

	SetSizer(textureViewerSizer);
	Layout();

	count++;
}

TextureViewerPanel::~TextureViewerPanel()
{
	count--;

	if (count <= 0)
	{
		delete vao;
	}
}

void TextureViewerPanel::DrawMGFTexture(const MATexture& texture)
{
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.Width()), 0, 0);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.Height()), 0, 1);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.depth), 0, 2);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.mips), 0, 3);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.frames), 0, 4);
	textureDetailsTable->SetTextValue(wxString::Format("%i", texture.format), 0, 5);
	textureDetailsTable->SetTextValue(wxString::Format("%02x", texture.flags), 0, 6);

	textureViewport->SetSize(wxSize(texture.Width(), texture.Height()));

	vao->Bind();
	textureview_shader->Use();
	texture.Bind(0);

	glViewport(0, 0, texture.Width(), texture.Height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

	textureViewport->SwapBuffers();
}

void TextureViewerPanel::SetGLContextCurrent()
{
	textureViewport->SetCurrent(Application::GetGLContext());
}

void TextureViewerPanel::InitGLCanvas()
{
	wxGLAttributes attribs;
	attribs.PlatformDefaults().MinRGBA(8, 8, 8, 8).DoubleBuffer().EndList();

	textureViewport = new wxGLCanvas(this, attribs, 15000, wxPoint(5, 60), wxSize(200, 200));
	textureViewport->SetCurrent(Application::GetGLContext());

	textureview_shader = Application::textureview_shader;

	if (count <= 0)
	{
		float textureViewBuffer[] = {
			 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 0.0f
		};

		unsigned char textureViewIndices[] = {
			0, 1, 2, 0, 2, 3
		};

		BufferLayout layout = {
			{ GLSLType::Vec3f, "in_position", 0 },
			{ GLSLType::Vec2f, "in_texCoord", 1 }
		};

		VertexBuffer textureViewVB(textureViewBuffer, sizeof(float) * 20, layout);
		IndexBuffer textureViewIB(textureViewIndices, 6, PrimitiveType::Triangles);

		vao = new VertexArray(textureViewVB, textureViewIB);
	}
}
