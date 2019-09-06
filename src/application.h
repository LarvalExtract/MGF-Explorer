#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <glad/glad.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <memory>

#include "renderer/Shader.h"
#include "mainwindow.h"
#include "mgf/MGFArchive.h"
#include "mgf/MGFTreeNode.h"
#include "mgf/MATexture.h"
#include "mgf/MGFModel.h"
#include "mgf/MAStrings.h"

class Application: public wxApp
{    
public:
    /// Constructor
    Application();
	~Application();

	void Log(const std::string& message);

    /// Initialises the application
    bool OnInit() override;

    /// Called on exit
    int OnExit() override;

	static const wxGLContext& GetGLContext() { return *oglContext; }

	static MATexture* GetTexture(const MGFTreeNode& textureNode);
	static MGFModel* GetModel(const MGFTreeNode& modelNode);
	static MAStrings* GetStrings(const MGFTreeNode& mgTextNode);

private:
	MainWindow* mainWindow;

	void InitOpenGL();
	void CreateStaticShaders();
	//void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	static wxGLContext* oglContext;

public:
	static std::shared_ptr<ShaderProgram> textureview_shader;
	static std::shared_ptr<ShaderProgram> shader_default;
	static std::shared_ptr<ShaderProgram> material_base_shader;
	static std::shared_ptr<ShaderProgram> material_complex_shader;

private:
	static std::unordered_map<std::string, MATexture> map_textures;
	static std::unordered_map<std::string, MGFModel> map_models;
	static std::unordered_map<std::string, MAStrings> map_strings;
};

#endif // _APPLICATION_H_
