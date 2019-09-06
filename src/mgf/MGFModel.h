#pragma once

#include "MGFTreeNode.h"
#include "MGFMesh.h"
#include "MGFMaterial.h"

#include <wx/dataview.h>

#include <fstream>
#include <vector>
#include <unordered_map>

class MGFModel : public wxDataViewVirtualListModel
{
public:
	MGFModel(const MGFTreeNode& node);
	~MGFModel();

public:
	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int col) const;
	virtual unsigned int GetCount() const override;
	virtual void GetValueByRow(wxVariant& variant, unsigned row, unsigned col) const override;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned row, unsigned col) override;

	std::vector<wxString> meshNames;
	std::unordered_map<std::string, MGFMesh> meshes;
	std::unordered_map<std::string, MGFMaterial*> materials;
private:

	// Recurse through the MGMODEL's XML scene graph and apply accumulated transforms to each mesh
	void TraverseXmlScene(const pugi::xml_node& node, const glm::mat4x4& parentTransform);
};