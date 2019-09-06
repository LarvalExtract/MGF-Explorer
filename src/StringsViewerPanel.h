#pragma once

#include "mgf/MAStrings.h"

#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/sizer.h>

class StringsViewerPanel : public wxPanel
{
public:
	StringsViewerPanel(wxWindow* parent);
	~StringsViewerPanel();

	void SetStrings(MAStrings* strings);

private:
	wxDataViewListCtrl* stringTable;
};