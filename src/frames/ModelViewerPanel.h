#pragma once

#include "../mgf/MGFModel.h"

#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/sizer.h>
#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/timer.h>

#include "../renderer/Shader.h"
#include "../renderer/VertexArray.h"

class ModelViewport : public wxGLCanvas
{
public:
	ModelViewport(wxWindow* parent, const wxGLAttributes& attribs);

	void UploadMeshes(const MGFModel& model);

	void ResizeViewport(int width, int height);
	void UpdateCamera(float deltaTime);
	void Draw();

private:
	void OnWindowResize(wxSizeEvent& event);
	void OnMouseRightClick(wxMouseEvent& event);
	void OnMouseRightRelease(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);

private:
	bool bCameraActive;
	wxPoint storedMousePosition;
	float moveSpeed;

	float horizontalAngle;
	float verticalAngle;

	glm::vec3 forward, right, up;

	struct Camera
	{
		glm::vec3 position;
		glm::mat4 projMatrix;
		glm::mat4 viewMatrix;
	} camera;

	std::vector<const MGFMesh*> meshQueue;

	wxDECLARE_EVENT_TABLE();
};

class ModelViewerPanel : public wxPanel
{
public:
	ModelViewerPanel(wxWindow* parent);

	void SetCurrentGLContext();
	void SetMGModel(MGFModel* model);
	void NameThisProperly(wxTimerEvent& event);

private:
	void OnHide(wxShowEvent& event);
	void OnMeshSelect(wxDataViewEvent& event);

private:
	wxNotebook* notebook;
	ModelViewport* modelView;
	wxGLCanvas* uvView;
	wxTimer timer;

	wxDataViewListCtrl* meshList;
	wxDataViewColumn* colMeshName;
	wxDataViewColumn* colVtxCount;
	wxDataViewColumn* colStride;
	wxDataViewColumn* colFlags;

private:
	MGFModel* mgModel;

	wxDECLARE_EVENT_TABLE();
};