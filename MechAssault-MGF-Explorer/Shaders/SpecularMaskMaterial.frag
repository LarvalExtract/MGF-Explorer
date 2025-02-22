#version 330 core

#pragma include Common.inc.frag

uniform sampler2D basetexture;
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
	vec4 diffuse = texture(basetexture, texCoord) * diffuse_colour;
	vec4 specular = vec4(spec_colour.xyz, texture(basetexture, texCoord).w); 
	vec4 ambient = amb_colour;

	vec3 worldView = normalize(eyePosition - worldPosition);

	vec4 lit = computeLighting(ambient, diffuse, specular, shininess, worldPosition, worldView, worldNormal);
	lit += diffuse * selfillum_colour;
	fragColour = lit;
}