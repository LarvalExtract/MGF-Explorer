#include "Shader.h"

#include <fstream>
#include <sstream>
#include <filesystem>

ShaderProgram* ShaderProgram::currentBoundShader = nullptr;

void Shader::CollectShaderSource(const std::filesystem::path& file_path, std::stringstream& shader_source)
{
	std::ifstream file(file_path);

	if (!file.is_open())
		return;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("#include") != std::string::npos)
		{
			std::size_t quote = line.find('"') + 1;
			std::size_t inc_file_length = line.length() - quote - 1;
			std::string inc_file_name = line.substr(quote, inc_file_length);

			std::filesystem::path inc_file_path = file_path.parent_path();
			inc_file_path.append(inc_file_name);

			CollectShaderSource(inc_file_path, shader_source);
		}
		else
		{
			shader_source << line << '\n';
		}
	}

	file.close();
}

int Shader::CompileShaderSource(const char* source)
{
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

	return result;
}

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
	std::filesystem::path path(filename);
	std::stringstream ss;

	CollectShaderSource(path, ss);

	if (CompileShaderSource(ss.str().c_str()) == GL_FALSE)
		return;

	bValid = true;
}

std::string Shader::GetCompileErrorLog()
{
	int logLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

	std::string log;
	log.reserve(logLength);
	glGetShaderInfoLog(shaderID, logLength, nullptr, &log[0]);

	return log;
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
	if (ShaderProgram::currentBoundShader == this)
		return;

	glUseProgram(shaderProgramID);
	ShaderProgram::currentBoundShader = this;
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

int ShaderProgram::GetUniformLocation(const std::string& uniformName)
{
	return glGetUniformLocation(shaderProgramID, uniformName.c_str());
}

void ShaderProgram::SetUniformBool(const std::string& name, bool value)
{
	SetUniformInt(name, value);
}

void ShaderProgram::SetUniformInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::BindAttributeLocation(unsigned int location, const std::string& attributeName)
{
	glBindAttribLocation(shaderProgramID, location, attributeName.c_str());
}

void ShaderProgram::SetUniformFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
	glUniform3fv(GetUniformLocation(name), 1, &vector[0]);
}

void ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4x4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::SetUniformSampler(const std::string& name, int textureUnit)
{
	SetUniformInt(name, textureUnit);
}

//std::string ShaderProgram::GetLinkErrorLog()
//{

//}
