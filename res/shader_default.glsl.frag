#version 430 core

out vec4 colour;

uniform bool u_selected = false;
uniform bool u_two_sided = false;

const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{
	if (u_two_sided == false)
		if (gl_FrontFacing == false)
			discard;

	if (u_selected == true)
		colour = mix(white, red, 1.0);
	else
		colour = mix(white, red, 0.0);
}