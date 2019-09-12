#pragma once

#include "MGFTreeNode.h"

#include <string>
#include <wx/dataview.h>

class MAStrings : public wxDataViewVirtualListModel
{
public:
	MAStrings(const MGFTreeNode& mgText);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int col) const;
	virtual unsigned int GetCount() const override;
	virtual void GetValueByRow(wxVariant& variant, unsigned row, unsigned col) const override;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned row, unsigned col) override;

private:
	std::vector<std::pair<long, wxString>> strings;
};