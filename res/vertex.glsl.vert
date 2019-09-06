#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texCoord;

out vec2 texCoord;

uniform bool u_scaleTexcoord;
uniform mat4 u_mvpMatrix;

void main()
{
	texCoord = in_texCoord;

	if (u_scaleTexcoord == true)
		texCoord /= vec2(1024.0);

	gl_Position = u_mvpMatrix * vec4(in_position, 1.0);
}