#pragma once

#include <unordered_map>
#include <string_view>
#include <QByteArray>

namespace Qt3DRender
{
	class QShaderProgram;
}

namespace MA
{
	class ShaderProgramProvider final
	{
	public:
		Qt3DRender::QShaderProgram* GetShaderProgram(uint32_t vertexFlags, std::string_view materialType);

	private:
		QByteArray LoadVertexShaderSource(uint32_t vertexFlags) const;
		QByteArray LoadFragmentShaderSource(std::string_view materialType) const;

		std::unordered_map<uint64_t, Qt3DRender::QShaderProgram*> mShaderPrograms;
	};
}