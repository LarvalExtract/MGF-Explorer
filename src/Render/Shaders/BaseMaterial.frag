#pragma include phong.inc.frag

uniform sampler2D basetexture;
uniform vec3 diffuse_colour;
uniform vec3 spec_colour;
uniform vec3 amb_colour;
uniform vec3 selfillum_colour;
uniform float shininess;

uniform vec3 eyePosition;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 worldNormal;

out vec4 fragColour;

void main()
{
	const vec4 diffuse = texture(basetexture, texCoord) * diffuse_colour;
	const vec4 specular = texture(basetexture, texCoord).wwww * spec_colour; // specular channel seems to be alpha in mechassault
	const vec4 ambient = amb_colour;

	const vec3 worldView = normalize(eyePosition - worldPosition);

	fragColour = phongFunction(ambient, diffuse, specular, shininess, worldPosition, worldView, worldNormal);
}