#include "NotebookPage.h"
#include "MyNotebook.h"

static const int treeViewWidth = 500;

wxBEGIN_EVENT_TABLE(NotebookPage, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(9000, OnItemSelected)
wxEND_EVENT_TABLE();

NotebookPage::NotebookPage(MyNotebook* notebook, MGFArchive* mgfFile) :
	wxPanel(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
	mgfArchive(*mgfFile)
{
	treeView = new wxDataViewCtrl(this, 9000, wxDefaultPosition, wxSize(treeViewWidth, -1), wxDV_NO_HEADER);
	treeView->SetMaxSize(wxSize(treeViewWidth, -1));

	wxDataViewIconTextRenderer* tr = new wxDataViewIconTextRenderer();
	wxDataViewColumn* col = new wxDataViewColumn(mgfFile->FileName(), tr, 0, treeViewWidth, wxALIGN_LEFT);

	dataView = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);
	dataView->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	dataView->Hide();

	textureViewer = new TextureViewerPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME);
	textureViewer->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	textureViewer->Hide();

	sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(treeView, 1, wxALL | wxEXPAND, 5);
	sizer->Add(dataView, 1, wxALL | wxEXPAND, 5);

	this->SetSizerAndFit(sizer);
	this->Layout();

	treeView->AppendColumn(col);
	treeView->AssociateModel(mgfFile);
	//mgfArchive.IncRef();
}

NotebookPage::~NotebookPage()
{

}

void NotebookPage::OnItemSelected(wxDataViewEvent& event)
{
	if (!treeView->GetSelection().IsOk())
		return;

	const MGFTreeNode& selectedNode = *static_cast<const MGFTreeNode*>(treeView->GetSelection().GetID());

	if (!selectedNode.IsFile())
	{
		sizer->Hide(sizer->GetItem(1)->GetWindow());
		return;
	}

	MyNotebook& parent = *static_cast<MyNotebook*>(this->GetParent());
	MGFResourceManager& mgfResources = parent.mgfResources;

	switch (selectedNode.FileType())
	{
	case MGFFileType::Texture:
		RenderTexture(*mgfResources.GetTexture(selectedNode), parent);
		break;

	case MGFFileType::None:
		textureViewer->Hide();
		sizer->Replace(sizer->GetItem(1)->GetWindow(), dataView);
		sizer->Show(dataView);
		break;
	}

	this->Layout();
	this->Update();
}

void NotebookPage::RenderTexture(const MGFTexture& texture, const MyNotebook& parent)
{
	textureViewer->SetCurrentContext(parent.GetGLContext());

	sizer->Replace(sizer->GetItem(1)->GetWindow(), textureViewer);
	sizer->Show(textureViewer);

	parent.textureViewShader->Use();
	parent.textureViewVAO->Bind();

	textureViewer->DrawMGFTexture(texture);
	textureViewer->Refresh();
}
