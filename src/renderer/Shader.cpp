#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(ShaderType type) :
	bValid(false)
{
	shaderID = glCreateShader(static_cast<GLint>(type));
}

Shader::~Shader()
{
	glDeleteShader(shaderID);
}

void Shader::LoadSourceFromString(const char* source)
{
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
		return;

	bValid = true;
}

void Shader::LoadSourceFromFile(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
		return;

	std::stringstream ss;
	ss << file.rdbuf();

	std::string sourceStr = ss.str();
	const char* source = sourceStr.c_str();

	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
		return;

	bValid = true;
}

std::string Shader::GetCompileErrorLog()
{
	int logLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

	std::string errorLog;
	errorLog.reserve(logLength);

	glGetShaderInfoLog(shaderID, logLength, nullptr, reinterpret_cast<GLchar*>(&errorLog[0]));

	return errorLog;
}



ShaderProgram::ShaderProgram()
{
	shaderProgramID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgramID);
}

void ShaderProgram::Use()
{
	glUseProgram(shaderProgramID);
}

void ShaderProgram::AttachShader(const Shader& vertexShader)
{
	if (!vertexShader.IsValid())
		return;

	glAttachShader(shaderProgramID, vertexShader.ID());
}

void ShaderProgram::LinkShaders()
{
	glLinkProgram(shaderProgramID);

	glBindAttribLocation(shaderProgramID, 0, "in_position");
	glBindAttribLocation(shaderProgramID, 1, "in_texCoord");
}

int ShaderProgram::GetAttributeLocation(const std::string& attributeName)
{
	return glGetAttribLocation(shaderProgramID, attributeName.c_str());
}

void ShaderProgram::BindAttributeLocation(unsigned int location, const std::string& attributeName)
{
	glBindAttribLocation(shaderProgramID, location, attributeName.c_str());
}

void ShaderProgram::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void ShaderProgram::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &vector[0]);
}

void ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4x4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

//std::string ShaderProgram::GetLinkErrorLog()
//{

//}
