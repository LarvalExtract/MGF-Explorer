#include "MyNotebook.h"

#include <wx/msgdlg.h>

MyNotebook::MyNotebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
	wxNotebook(parent, id, pos, size)
{


	bGLInitialised = InitOpenGL();
}

MyNotebook::~MyNotebook()
{
	CloseAllMgfFiles();
}

void MyNotebook::OpenMgfFile(const std::filesystem::path& archiveFilepath)
{
	MGFArchive* newArchive = new MGFArchive(archiveFilepath);
	mapArchives.insert(std::make_pair(newArchive->FileName().ToStdString(), newArchive));

	this->AddPage(new NotebookPage(this, newArchive), newArchive->FileName(), true);
}

void MyNotebook::CloseMgfFile(int index)
{
	const std::string& archiveName = static_cast<NotebookPage*>(this->GetPage(index))->GetMGFFile().FileName().ToStdString();

	this->DeletePage(index);

	delete mapArchives[archiveName];

	mapArchives.erase(archiveName);
}

void MyNotebook::CloseAllMgfFiles()
{
	this->DeleteAllPages();

	for (auto& archive : mapArchives)
		delete archive.second;

	mapArchives.clear();
}

bool MyNotebook::MgfFileIsOpen(const std::string& archiveName)
{
	return mapArchives.find(archiveName) != mapArchives.end();
}

void MyNotebook::SelectMgfFile(const std::string& archiveName)
{
	int index = 0;
	for (auto& mgfFile : mapArchives)
	{
		if (mgfFile.second->FileName().ToStdString() == archiveName)
		{
			this->SetSelection(index);
			break;
		}
		index++;
	}
}

bool MyNotebook::InitOpenGL()
{
	wxGLAttributes canvasAttributes;
	canvasAttributes.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	wxGLCanvas tempCanvas(this, canvasAttributes, 15000);

	wxGLContextAttrs contextAttributes;
	contextAttributes.PlatformDefaults().CoreProfile().OGLVersion(4, 0).EndList();

	oglContext = new wxGLContext(&tempCanvas, nullptr, &contextAttributes);
	oglContext->SetCurrent(tempCanvas);

	int result = gladLoadGL();

	if (result == GL_FALSE || !oglContext->IsOK())
	{
		wxMessageBox("Failed to initialise OpenGL!");
		return false;
	}

	textureViewShader = std::make_unique<ShaderProgram>();

	Shader vertexShader(ShaderType::Vertex);
	vertexShader.LoadSourceFromFile("res/textureview.vert");

	Shader fragmentShader(ShaderType::Fragment);
	fragmentShader.LoadSourceFromFile("res/textureview.frag");

	textureViewShader->AttachShader(vertexShader);
	textureViewShader->AttachShader(fragmentShader);
	textureViewShader->LinkShaders();

	textureViewShader->BindAttributeLocation(0, "in_position");
	textureViewShader->BindAttributeLocation(1, "in_texCoord");

	float textureViewBuffer[] = {
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned char textureViewIndices[] = {
		0, 1, 2, 0, 2, 3
	};

	BufferLayout layout = {
		{ GLSLType::Vec3f, "in_position" },
		{ GLSLType::Vec2f, "in_texCoord" }
	};

	VertexBuffer textureViewVB(textureViewBuffer, sizeof(float) * 20, layout);
	IndexBuffer textureViewIB(textureViewIndices, 6, PrimitiveType::Triangles);

	textureViewVAO = std::make_unique<VertexArray>(textureViewVB, textureViewIB);

	return true;
}
