///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"

#include "renderer/Buffers.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(10001, OnOpenFileClicked)
	EVT_MENU(10002, OnCloseFileClicked)
	EVT_MENU(10003, OnCloseAllFilesClicked)
	EVT_NOTEBOOK_PAGE_CHANGED(10500, OnPageSelectionChanged)
wxEND_EVENT_TABLE();

MainWindow::MainWindow() :
	wxFrame(nullptr, wxID_ANY, "MechAssault MGF Explorer", wxDefaultPosition, wxSize(1280, 720), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL),
	panel_mainWindow(nullptr),
	mgfNotebook(nullptr),
	menuBar(nullptr)
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );
	SetMinSize(wxSize(640, 480));

	wxBoxSizer* sizer_mainWindow;
	sizer_mainWindow = new wxBoxSizer( wxVERTICAL );

	panel_mainWindow = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizer_notebook;
	sizer_notebook = new wxBoxSizer( wxVERTICAL );

	mgfNotebook = new MyNotebook(panel_mainWindow, 10500, wxDefaultPosition, wxDefaultSize);
	mgfNotebook->Hide();

	sizer_notebook->Add( mgfNotebook, 1, wxEXPAND | wxALL, 0 );

	panel_mainWindow->SetSizer(sizer_notebook);
	panel_mainWindow->Layout();

	InitStatusBar();

	sizer_notebook->Fit(panel_mainWindow);
	sizer_mainWindow->Add(panel_mainWindow, 1, wxEXPAND | wxALL, 0);
	sizer_mainWindow->Add(statusBarPanel, 1, wxEXPAND | wxALL, 0);

	InitMenuBar();

	SetSizer( sizer_mainWindow );
	Layout();

	Centre( wxBOTH );
}

MainWindow::~MainWindow()
{
	delete panel_mainWindow;
}


void MainWindow::OnOpenFileClicked(wxCommandEvent& event)
{
	if (auto openFile = wxFileDialog(this, "Open MechAssault 2 MGF file", "", "", "MGF files (.mgf)|*.mgf", wxFD_OPEN | wxFD_FILE_MUST_EXIST); openFile.ShowModal() != wxID_CANCEL)
	{
		if (const std::filesystem::path filepath = openFile.GetPath().ToStdString(); std::filesystem::exists(filepath))
		{
			if (const auto filename = filepath.filename().string(); mgfNotebook->MgfFileIsOpen(filename))
				mgfNotebook->SelectMgfFile(filename);
			else
				mgfNotebook->OpenMgfFile(filepath);

			mgfNotebook->Show();

			panel_mainWindow->Layout();
		}
	}
}


void MainWindow::OnCloseFileClicked(wxCommandEvent& event)
{
	if (mgfNotebook->GetPageCount() > 0)
		mgfNotebook->CloseMgfFile(mgfNotebook->GetSelection());

	if (mgfNotebook->GetPageCount() == 0)
		mgfNotebook->Hide();
}


void MainWindow::OnCloseAllFilesClicked(wxCommandEvent& event)
{
	mgfNotebook->CloseAllMgfFiles();

	mgfNotebook->Hide();

	statusBarLabel->SetLabelText("");
}


void MainWindow::OnPageSelectionChanged(wxBookCtrlEvent& event)
{
	auto page = static_cast<NotebookPage*>(mgfNotebook->GetPage(mgfNotebook->GetSelection()));
	page->OnItemSelected(reinterpret_cast<wxDataViewEvent&>(event));

	const auto& archive = page->GetMGFFile();

	menuBar->GetMenu(0)->GetMenuItems()[1]->SetItemLabel("Close " + page->GetMGFFile().FileName());

	statusBarLabel->SetLabelText(archive.FileName() + " | " + ToFileSizeStr(archive.Size()) + " | " + std::to_string(archive.FileCount()) + " files in archive");
}

void MainWindow::InitMenuBar()
{
	auto fileMenu = new wxMenu;
	fileMenu->Append(10001, "Open MGF file...", "", wxITEM_NORMAL);
	fileMenu->Append(10002, "Close", "", wxITEM_NORMAL);
	fileMenu->Append(10003, "Close all", "", wxITEM_NORMAL);
	fileMenu->Append(wxID_ANY, "", "", wxITEM_SEPARATOR);
	fileMenu->Append(10004, "Launch batch extractor...", "", wxITEM_NORMAL);

	auto aboutMenu = new wxMenu;
	
	menuBar = new wxMenuBar(0);
	menuBar->Append(fileMenu, "File");
	menuBar->Append(aboutMenu, "About");

	SetMenuBar(menuBar);
}

void MainWindow::InitStatusBar()
{
	statusBarPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	statusBarPanel->SetMaxSize(wxSize(-1, 25));
	
	wxBoxSizer* statusBarSizer = new wxBoxSizer(wxHORIZONTAL);

	statusBarLabel = new wxStaticText(statusBarPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	statusBarSizer->Add(statusBarLabel, 1, wxALL | wxEXPAND, 4);

	statusBarPanel->SetSizer(statusBarSizer);
	statusBarSizer->Layout();
	statusBarSizer->Fit(statusBarPanel);
}

wxString MainWindow::ToFileSizeStr(unsigned int bytes)
{
	wxString result;

	constexpr float megabyte = 1024 * 1024;
	constexpr float kilobyte = 1024;

	if (bytes > megabyte)
	{
		float sizeInMB = (static_cast<float>(bytes) / megabyte);
		unsigned int rounded = (sizeInMB * 100.0f) + 0.5f;
		result << rounded << "MB";
		result.insert(2, '.');
	}
	else if (bytes > kilobyte)
	{
		float sizeInKB = (static_cast<float>(bytes) / kilobyte);
		unsigned int rounded = (sizeInKB * 100.0f) + 0.5f;
		result << rounded << "kB";
		result.insert(2, '.');
	}
	else
	{
		result << bytes << " bytes";
	}

	return result;
}
