#version 430 core

#include "shared.glsl.inc"

uniform sampler2D u_basetexture;

void main()
{
	CullBackface();

	vec4 texel = texture(u_basetexture, texCoord);
	colour = texel;

	if (u_selected == true)
		colour = mix(colour, yellow, 0.5);
}