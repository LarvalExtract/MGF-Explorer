#pragma once

#include <glm/vec3.hpp>
#include "VertexArray.h"

namespace Shape
{
	class Grid
	{
	public:
		Grid();

	private:
		glm::vec3 position;

		int segments_x;
		int segments_z;
		float spacing;

		VertexArray vao;
	};
}