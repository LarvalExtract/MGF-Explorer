#include "NotebookPage.h"
#include "application.h"

static const int treeViewWidth = 620;

wxBEGIN_EVENT_TABLE(NotebookPage, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(9000, OnItemSelected)
wxEND_EVENT_TABLE();

NotebookPage::NotebookPage(wxWindow* parent, MGFArchive* mgfFile) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
	mgfArchive(*mgfFile)
{
	treeView = new wxDataViewCtrl(this, 9000, wxDefaultPosition, wxSize(treeViewWidth, -1));
	treeView->SetMaxSize(wxSize(treeViewWidth, -1));

	textureViewer = new TextureViewerPanel(this);
	textureViewer->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	textureViewer->Hide();

	modelViewer = new ModelViewerPanel(this);
	modelViewer->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	modelViewer->Hide();

	stringsViewer = new StringsViewerPanel(this);
	stringsViewer->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
	stringsViewer->Hide();

	placeholderPanel = new wxPanel(this);

	sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(treeView, 1, wxALL | wxEXPAND, 5);
	sizer->Add(placeholderPanel, 1, wxALL | wxEXPAND, 5);

	SetSizerAndFit(sizer);
	Layout();

	treeView->AppendColumn(new wxDataViewColumn("Name", new wxDataViewIconTextRenderer(), 0, 360, wxALIGN_LEFT));
	treeView->AppendColumn(new wxDataViewColumn("Date", new wxDataViewTextRenderer(), 1, 105, wxALIGN_LEFT));
	treeView->AppendColumn(new wxDataViewColumn("Type", new wxDataViewTextRenderer(), 2, 65, wxALIGN_LEFT));
	treeView->AppendColumn(new wxDataViewColumn("Size", new wxDataViewTextRenderer(), 3, 90, wxALIGN_LEFT));
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

	switch (selectedNode.FileType())
	{
	case MGFFileType::Texture:
		ShowPanel(textureViewer);
		textureViewer->SetGLContextCurrent();
		textureViewer->DrawMGFTexture(*Application::GetTexture(selectedNode));
		break;

	case MGFFileType::Model:
		ShowPanel(modelViewer);
		modelViewer->SetCurrentGLContext();
		modelViewer->SetMGModel(Application::GetModel(selectedNode));
		break;

	case MGFFileType::Strings:
		ShowPanel(stringsViewer);
		stringsViewer->SetStrings(Application::GetStrings(selectedNode));
		break;

	case MGFFileType::None:
		ShowPanel(nullptr);
		break;
	}

	Layout();
	Update();
}

void NotebookPage::ShowPanel(wxWindow* newPanel)
{
	wxWindow* currentPanel = sizer->GetItem(1)->GetWindow();

	// If newPanel is nullptr, this means hide the current panel
	if (newPanel == nullptr)
	{
		// Do nothing
		currentPanel->Hide();

		return;
	}

	// Otherwise, show new panel
	else
	{
		// Desired panel is the same as current, exit
		if (newPanel == currentPanel)
		{
			newPanel->Show();
		}
		else
		{
			// Swap current panel with new one
			currentPanel->Hide();
			sizer->Replace(currentPanel, newPanel);
			newPanel->Show();
		}

		sizer->Layout();
	}
}
