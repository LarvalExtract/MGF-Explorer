#include "ShaderProgramProvider.h"

#include <QShaderProgram>
#include <QFile>

using namespace MA;
using namespace Qt3DRender;

static const QString sShaderSourceRoot = "shaders/";

QShaderProgram* ShaderProgramProvider::GetShaderProgram(uint32_t vertexFlags, std::string_view materialType)
{
	std::hash<std::string_view> materialTypeHash;

	// This doesn't seem good...
	const uint64_t key =
		static_cast<uint64_t>(vertexFlags) << 32 |
		materialTypeHash(materialType) & 0x00000000FFFFFFFFll;

	if (mShaderPrograms.contains(key))
	{
		return mShaderPrograms.at(key);
	}

	QShaderProgram* shader = new QShaderProgram;
	shader->setFormat(QShaderProgram::GLSL);
	shader->setVertexShaderCode(LoadVertexShaderSource(vertexFlags));
	shader->setFragmentShaderCode(LoadFragmentShaderSource(materialType));

	mShaderPrograms.insert(std::make_pair(key, shader));

	return shader;
}

QByteArray ShaderProgramProvider::LoadVertexShaderSource(uint32_t vertexFlags) const
{
	static const std::unordered_map<uint32_t, const char*> vertexShaderMap = {
	//	{, ".glsl"},
	//	{, ".glsl"}
	};

	QString filename = sShaderSourceRoot + vertexShaderMap.at(vertexFlags);
	QFile shaderFile(filename);

	return shaderFile.readAll();
}

QByteArray ShaderProgramProvider::LoadFragmentShaderSource(std::string_view materialType) const
{
	return QByteArray();
}
