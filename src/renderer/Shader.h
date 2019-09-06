#pragma once

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <filesystem>
#include <sstream>

enum class ShaderType : GLint
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

// Base shader class for VertexShader, FragmentShader, etc.
// You should not be using this directly
class Shader
{
public:
	Shader(ShaderType type);
	~Shader();

	void LoadSourceFromString(const char* source);
	void LoadSourceFromFile(const std::string& filename);

	inline unsigned int ID() const { return shaderID; }
	inline bool IsValid() const { return bValid; }

	std::string GetCompileErrorLog();

private:	
	unsigned int shaderID;
	bool bValid;

	void CollectShaderSource(const std::filesystem::path& file_path, std::stringstream& shader_source);
	int CompileShaderSource(const char* source);
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void Use();

	void AttachShader(const Shader& vertexShader);
	void LinkShaders();

	void BindAttributeLocation(unsigned int location, const std::string& attributeName);
	int GetAttributeLocation(const std::string& attributeName);

	int GetUniformLocation(const std::string& uniformName);

	void SetUniformBool(const std::string& name, bool value);
	void SetUniformInt(const std::string& name, int value);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
	void SetUniformMat4(const std::string& name, const glm::mat4x4& matrix);

	void SetUniformSampler(const std::string& name, int textureUnit);

	//std::string GetLinkErrorLog();

private:
	unsigned int shaderProgramID;

	static ShaderProgram* currentBoundShader;
};

