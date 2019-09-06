#include "application.h"

#include <chrono>

IMPLEMENT_APP(Application)

wxGLContext* Application::oglContext = nullptr;

std::shared_ptr<ShaderProgram> Application::textureview_shader;
std::shared_ptr<ShaderProgram> Application::shader_default;
std::shared_ptr<ShaderProgram> Application::material_base_shader;
std::shared_ptr<ShaderProgram> Application::material_complex_shader;

std::unordered_map<std::string, MATexture> Application::map_textures;
std::unordered_map<std::string, MGFModel> Application::map_models;
std::unordered_map<std::string, MAStrings> Application::map_strings;

Application::Application()
{
    
}

Application::~Application()
{

}

static void to_lower(std::string& str)
{
	for (std::size_t i = 0; i < str.size(); i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
}

void Application::Log(const std::string& message)
{
	//mainWindow->TextOutput() << "[00:00:00]\t" <<
}

bool Application::OnInit()
{    
	mainWindow = new MainWindow();
	mainWindow->Show(true);

	InitOpenGL();

    return true;
}

int Application::OnExit()
{    
	delete oglContext;

	return wxApp::OnExit();
}

MATexture* Application::GetTexture(const MGFTreeNode& textureNode)
{
	std::string path = textureNode.GetFullPath();
	to_lower(path); 

	if (map_textures.find(path) == map_textures.end())
		map_textures.try_emplace(path, textureNode);

	return &map_textures.at(path);
}

MGFModel* Application::GetModel(const MGFTreeNode& modelNode)
{
	std::string path = modelNode.GetFullPath();
	to_lower(path);

	if (map_models.find(path) == map_models.end())
		map_models.try_emplace(path, modelNode);

	return &map_models.at(path);
}

MAStrings* Application::GetStrings(const MGFTreeNode& mgTextNode)
{
	std::string path = mgTextNode.GetFullPath();
	to_lower(path);

	if (map_strings.find(path) == map_strings.end())
		map_strings.try_emplace(path, mgTextNode);

	return &map_strings.at(path);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	MainWindow* window = (MainWindow*)userParam;

	wxString strSource, strType, strSeverity;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				strSource = "API"; break;
	case GL_DEBUG_SOURCE_APPLICATION:		strSource = "APPLICATION"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		strSource = "WINDOW_SYSTEM"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	strSource = "SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		strSource = "THIRD_PARTY"; break;
	case GL_DEBUG_SOURCE_OTHER:				strSource = "OTHER"; break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				strType = "ERROR"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: strType = "DEPRECATED_BEHAVIOUR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  strType = "UNDEFINED_BEHAVIOUR"; break;
	case GL_DEBUG_TYPE_PORTABILITY:			strType = "PORTABILITY"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:			strType = "PERFORMANCE"; break;
	case GL_DEBUG_TYPE_OTHER:				strType = "OTHER"; break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			strSeverity = "HIGH"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:			strSeverity = "MEDIUM"; break;
	case GL_DEBUG_SEVERITY_LOW:				strSeverity = "LOW"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	strSeverity = "NOTIFICATION"; break;
	}

	window->TextOutput()
		<< "\n[OpenGL]\n"
		<< "Source:\t" << strSource << '\n'
		<< "Type:\t" << strType << '\n'
		<< "Severity:\t" << strSeverity << '\n'
		<< "Message:\t" << message << '\n';
}

void Application::InitOpenGL()
{
	wxGLAttributes attribs;
	attribs.PlatformDefaults().MinRGBA(8,8,8,8).DoubleBuffer().Depth(16).EndList();

	wxGLCanvas tempCanvas(mainWindow, attribs, wxID_ANY);

	wxGLContextAttrs contextAttributes;
	contextAttributes.CoreProfile().OGLVersion(4, 3).EndList();

	Application::oglContext = new wxGLContext(&tempCanvas, nullptr, &contextAttributes);
	bool a = oglContext->SetCurrent(tempCanvas);

	if (!oglContext->IsOK())
	{
		mainWindow->TextOutput() << "Failed to create OpenGL context!";
		return;
	}

	// Initialise glad
	int result = gladLoadGL();

	if (result != GL_TRUE)
		mainWindow->TextOutput() << "Failed to initialise OpenGL!\n";
	else
	{
		int major = 0, minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		mainWindow->TextOutput()
			<< "OpenGL " << major << '.' << minor << "\n"
			<< "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n'
			<< "Vendor: " << glGetString(GL_VENDOR) << '\n'
			<< "Renderer: " << glGetString(GL_RENDERER) << '\n';
	}

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(&MessageCallback), mainWindow);
	//glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, true);

	CreateStaticShaders();
}

void Application::CreateStaticShaders()
{
	{
		Shader textureViewVertexShader(ShaderType::Vertex);
		textureViewVertexShader.LoadSourceFromFile("res/textureview.glsl.vert");
		Shader textureViewFragmentShader(ShaderType::Fragment);
		textureViewFragmentShader.LoadSourceFromFile("res/textureview.glsl.frag");

		Application::textureview_shader = std::make_shared<ShaderProgram>();
		textureview_shader->AttachShader(textureViewVertexShader);
		textureview_shader->AttachShader(textureViewFragmentShader);
		textureview_shader->LinkShaders();

		textureview_shader->BindAttributeLocation(0, "in_position");
		textureview_shader->BindAttributeLocation(1, "in_texCoord");
		textureview_shader->Use();
		textureview_shader->SetUniformSampler("u_texture", 0);
	}
	
	Shader vertexShader(ShaderType::Vertex);
	vertexShader.LoadSourceFromFile("res/vertex.glsl.vert");
	mainWindow->TextOutput() << vertexShader.GetCompileErrorLog();

	{
		Shader shader_default_frag(ShaderType::Fragment);
		shader_default_frag.LoadSourceFromFile("res/shader_default.glsl.frag");
		mainWindow->TextOutput() << shader_default_frag.GetCompileErrorLog();

		Application::shader_default = std::make_shared<ShaderProgram>();
		shader_default->AttachShader(vertexShader);
		shader_default->AttachShader(shader_default_frag);
		shader_default->LinkShaders();

		shader_default->BindAttributeLocation(0, "in_position");
		shader_default->BindAttributeLocation(1, "in_texCoord");
	}
	{
		Shader material_base_frag(ShaderType::Fragment);
		material_base_frag.LoadSourceFromFile("res/material_base.glsl.frag");
		mainWindow->TextOutput() << material_base_frag.GetCompileErrorLog();

		Application::material_base_shader = std::make_shared<ShaderProgram>();
		material_base_shader->AttachShader(vertexShader);
		material_base_shader->AttachShader(material_base_frag);
		material_base_shader->LinkShaders();

		material_base_shader->BindAttributeLocation(0, "in_position");
		material_base_shader->BindAttributeLocation(1, "in_texCoord");
		material_base_shader->Use();
		material_base_shader->SetUniformSampler("u_basetexture", 0);
	}
	{
		Shader material_complex_frag(ShaderType::Fragment);
		material_complex_frag.LoadSourceFromFile("res/material_complex.glsl.frag");
		mainWindow->TextOutput() << material_complex_frag.GetCompileErrorLog();

		Application::material_complex_shader = std::make_shared<ShaderProgram>();
		material_complex_shader->AttachShader(vertexShader);
		material_complex_shader->AttachShader(material_complex_frag);
		material_complex_shader->LinkShaders();

		material_complex_shader->BindAttributeLocation(0, "in_position");
		material_complex_shader->BindAttributeLocation(1, "in_texCoord");
		material_complex_shader->Use();
		material_complex_shader->SetUniformSampler("u_basetexture", 0);
		material_complex_shader->SetUniformSampler("u_multitexture", 1);
	}
}

