#version 330 core

#pragma include Common.inc.frag

uniform vec4 diffuse_colour;
uniform vec4 spec_colour;
uniform vec4 amb_colour;
uniform vec4 selfillum_colour;
uniform float shininess;

uniform vec3 eyePosition;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 worldNormal;

out vec4 fragColour;

void main()
{
	vec4 diffuse = diffuse_colour;
	vec4 specular = spec_colour;
	vec4 ambient = amb_colour;

	vec3 worldView = normalize(eyePosition - worldPosition);

	vec4 lit = computeLighting(ambient, diffuse, specular, shininess, worldPosition, worldView, worldNormal);
	lit += diffuse * selfillum_colour;
	fragColour = lit;
}