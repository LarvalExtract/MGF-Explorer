#include "StringsViewerPanel.h"

StringsViewerPanel::StringsViewerPanel(wxWindow* parent) :
	wxPanel(parent)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	stringTable = new wxDataViewListCtrl(this, wxID_ANY);
	stringTable->AppendTextColumn(wxEmptyString, wxDATAVIEW_CELL_INERT, 35);
	stringTable->AppendTextColumn("ID", wxDATAVIEW_CELL_INERT, 65);
	stringTable->AppendTextColumn("String", wxDATAVIEW_CELL_INERT);

	sizer->Add(stringTable, 1, wxEXPAND | wxALL, 0);

	SetSizerAndFit(sizer);
	Layout();
}

void StringsViewerPanel::SetStrings(MAStrings* strings)
{
	stringTable->AssociateModel(strings);
}

