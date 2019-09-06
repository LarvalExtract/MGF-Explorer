///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <glad/glad.h>

#include "MyNotebook.h"
#include "NotebookPage.h"

#include <wx/dataview.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/textctrl.h>

#include <vector>
#include <unordered_map>
#include <memory>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainWindow
///////////////////////////////////////////////////////////////////////////////
class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow();

	wxTextCtrl& TextOutput() { return *textOutput; }

private:
	wxPanel* panel_mainWindow;
	MyNotebook* mgfNotebook;
	wxMenuBar* menuBar;
	wxPanel* statusBarPanel;
	wxStaticText* statusBarLabel;
	wxTextCtrl* textOutput;

	wxString defaultMgfLocation;

// Events
private:
	void OnOpenFileClicked(wxCommandEvent& event);
	void OnCloseFileClicked(wxCommandEvent& event);
	void OnCloseAllFilesClicked(wxCommandEvent& event);

	void OnPageSelectionChanged(wxBookCtrlEvent& event);

// Methods
private:
	void InitMenuBar();
	void InitStatusBar();

	wxString ToFileSizeStr(unsigned int bytes);

	wxDECLARE_EVENT_TABLE();
};

