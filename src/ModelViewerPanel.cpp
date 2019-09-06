#include "ModelViewerPanel.h"

#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int timerId = 16000;
const int meshListId = 17000;

wxBEGIN_EVENT_TABLE(ModelViewerPanel, wxPanel)
	EVT_TIMER(timerId, NameThisProperly)	
	EVT_SHOW(OnHide)
	EVT_DATAVIEW_SELECTION_CHANGED(meshListId, OnMeshSelect)
wxEND_EVENT_TABLE();

wxBEGIN_EVENT_TABLE(ModelViewport, wxGLCanvas)
	EVT_SIZE(OnWindowResize)
	EVT_RIGHT_DOWN(OnMouseRightClick)
	EVT_RIGHT_UP(OnMouseRightRelease)
	EVT_MOTION(OnMouseMove)
	EVT_PAINT(OnPaint)
wxEND_EVENT_TABLE();

ModelViewport::ModelViewport(wxWindow* parent, const wxGLAttributes& attribs) :
	wxGLCanvas(parent, attribs, wxID_ANY),
	bCameraActive(false),
	horizontalAngle(0.0f),
	verticalAngle(0.0f),
	forward({ 0, 0, 1 }),
	right({ 0, -1, 0 }),
	up({ 0, 1, 0 }),
	moveSpeed(0.03f)
{
	
}

ModelViewerPanel::ModelViewerPanel(wxWindow* parent) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_THEME),
	timer(this, timerId),
	mgModel(nullptr)
{
	notebook = new wxNotebook(this, wxID_ANY);

	wxGLAttributes attribs;
	attribs.PlatformDefaults().MinRGBA(8, 8, 8, 8).DoubleBuffer().Depth(16).EndList();

	modelView = new ModelViewport(notebook, attribs);
	SetCurrentGLContext();

	//uvView = new wxGLCanvas(notebook, attribs, wxID_ANY, wxDefaultPosition, wxSize(512, 512));

	notebook->AddPage(modelView, "3D View", true);
	//notebook->AddPage(uvView, "UV");

	meshList = new wxDataViewListCtrl(this, meshListId, wxDefaultPosition, wxSize(300, -1));
	meshList->SetMinSize(wxSize(350, -1));
	meshList->SetMaxSize(wxSize(350, -1));
	meshList->AppendTextColumn("Name", wxDATAVIEW_CELL_INERT, 176, wxALIGN_LEFT, 0);
	meshList->AppendTextColumn("Verts",	wxDATAVIEW_CELL_INERT, 50, wxALIGN_LEFT, 0);
	meshList->AppendTextColumn("Stride", wxDATAVIEW_CELL_INERT, 50, wxALIGN_LEFT, 0);
	meshList->AppendTextColumn("Flags",	wxDATAVIEW_CELL_INERT, 70, wxALIGN_LEFT, 0);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(notebook, 1, wxALL | wxEXPAND);
	sizer->Add(meshList, 1, wxALL | wxEXPAND);

	SetSizerAndFit(sizer);
	Layout();
}

ModelViewerPanel::~ModelViewerPanel()
{

}

void ModelViewerPanel::SetCurrentGLContext()
{
	modelView->SetCurrent(Application::GetGLContext());
}

void ModelViewerPanel::SetMGModel(MGFModel* model)
{
	mgModel = model;
	meshList->AssociateModel(mgModel);
	modelView->UploadMeshes(*mgModel);
	modelView->ResizeViewport(modelView->GetSize().GetWidth(), modelView->GetSize().GetHeight());
	
	timer.Start(16);
}

void ModelViewerPanel::NameThisProperly(wxTimerEvent& event)
{
	if (mgModel == nullptr)
		return;

	modelView->UpdateCamera(static_cast<float>(timer.GetInterval()) + 0.001f);
	modelView->Refresh();
}

void ModelViewport::UploadMeshes(const MGFModel& model)
{
	meshQueue.clear();
	meshQueue.reserve(model.meshes.size());

	for (const auto& mesh : model.meshes)
		meshQueue.push_back(&mesh.second);
	
	std::sort(meshQueue.begin(), meshQueue.end(), [](const MGFMesh* a, const MGFMesh* b) -> bool { return *a < *b; });
}

void ModelViewport::ResizeViewport(int width, int height)
{
	camera.projMatrix = glm::perspective(glm::radians(60.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	glViewport(0, 0, width, height);
}

void ModelViewport::UpdateCamera(float deltaTime)
{
	// handle keyboard movement
	if (bCameraActive)
	{
		if (wxGetKeyState(static_cast<wxKeyCode>('w')))
			camera.position += forward * moveSpeed * deltaTime;

		if (wxGetKeyState(static_cast<wxKeyCode>('s')))
			camera.position -= forward * moveSpeed * deltaTime;

		if (wxGetKeyState(static_cast<wxKeyCode>('a')))
			camera.position -= right * moveSpeed * deltaTime;

		if (wxGetKeyState(static_cast<wxKeyCode>('d')))
			camera.position += right * moveSpeed * deltaTime;

		if (wxGetKeyState(wxKeyCode::WXK_SPACE))
			camera.position += up * moveSpeed * deltaTime;

		if (wxGetKeyState(wxKeyCode::WXK_CONTROL))
			camera.position -= up * moveSpeed * deltaTime;
	}

	camera.viewMatrix = glm::lookAt(camera.position, camera.position + forward, up);
}

void ModelViewport::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 vpMatrix = camera.projMatrix * camera.viewMatrix;

	for (std::size_t i = 0; i < meshQueue.size(); i++)
	{
		meshQueue[i]->vao.Bind();
		meshQueue[i]->mat->Apply(vpMatrix * meshQueue[i]->transform);
		meshQueue[i]->mat->shader->SetUniformBool("u_selected", meshQueue[i]->bSelected);
		meshQueue[i]->mat->shader->SetUniformBool("u_scaleTexcoord", meshQueue[i]->bScaleTexCoords);

		glDrawElements(static_cast<GLenum>(meshQueue[i]->vao.GetIndexMode()), meshQueue[i]->vao.GetIndexCount(), GL_UNSIGNED_SHORT, 0);
	}

	glClearColor(0.62f, 0.73f, 0.86f, 1.0f);
}

void ModelViewport::OnWindowResize(wxSizeEvent& event)
{
	int newWidth = event.GetSize().GetWidth();
	int newHeight = event.GetSize().GetHeight();

	if (newWidth != 0 && newHeight != 0)
		ResizeViewport(newWidth, newHeight);
}

void ModelViewport::OnMouseRightClick(wxMouseEvent& event)
{
	bCameraActive = true;
	storedMousePosition = wxGetMousePosition();
	ShowCursor(false);
}

void ModelViewport::OnMouseRightRelease(wxMouseEvent& event)
{
	bCameraActive = false;
	SetCursorPos(storedMousePosition.x, storedMousePosition.y);
	ShowCursor(true);
}

void ModelViewport::OnMouseMove(wxMouseEvent& event)
{
	if (!bCameraActive)
		return;

	static const float mouseSpeed = 0.002f;

	const int halfWidth = GetSize().GetWidth() / 2;
	const int halfHeight = GetSize().GetHeight() / 2;

	wxPoint mousePos = ScreenToClient(wxGetMousePosition());

	horizontalAngle += mouseSpeed * static_cast<float>(halfWidth - mousePos.x);
	verticalAngle += mouseSpeed * static_cast<float>(halfHeight - mousePos.y);

	constexpr float halfPi = glm::half_pi<float>();
	constexpr float doublePi = glm::two_pi<float>();

	horizontalAngle = glm::mod<float>(horizontalAngle, doublePi);
	verticalAngle = glm::clamp(verticalAngle, -halfPi, halfPi);

	forward.x = glm::cos(verticalAngle) * glm::sin(horizontalAngle);
	forward.y = glm::sin(verticalAngle);
	forward.z = glm::cos(verticalAngle) * glm::cos(horizontalAngle);

	right.x = glm::sin(horizontalAngle - halfPi);
	right.y = 0.0f;
	right.z = glm::cos(horizontalAngle - halfPi);

	up = glm::cross(right, forward);

	wxPoint viewportCenter;
	ClientToScreen(&viewportCenter.x, &viewportCenter.y);
	viewportCenter.x += halfWidth;
	viewportCenter.y += halfHeight;

	SetCursorPos(viewportCenter.x, viewportCenter.y);
}

void ModelViewport::OnPaint(wxPaintEvent& event)
{
	wxPaintDC(this);

	if (meshQueue.size() == 0)
		return;

	Draw();

	SwapBuffers();
}

void ModelViewerPanel::OnHide(wxShowEvent& event)
{
	if (!event.IsShown())
		timer.Stop();
	else
		if (mgModel != nullptr)
			timer.Start();
}

void ModelViewerPanel::OnMeshSelect(wxDataViewEvent& event)
{
	if (!meshList->GetSelection().IsOk())
		return;

	static int lastRow = 0;
	int row = meshList->GetSelectedRow();

	MGFMesh& selectedMesh = mgModel->meshes.at(mgModel->meshNames[row].ToStdString());
	MGFMesh& deselectedMesh = mgModel->meshes.at(mgModel->meshNames[lastRow].ToStdString());

	selectedMesh.bSelected = true;
	deselectedMesh.bSelected = false;

	lastRow = row;
}
