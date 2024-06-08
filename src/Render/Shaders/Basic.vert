#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in uint packedNormal;
in vec2 vertexTexCoord;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 modelViewProjection;

vec3 unpackNormal(uint inPackedNormal)
{
	uint x = inPackedNormal >> 21;
	uint y = inPackedNormal >> 11 & 0x7FF;
	uint z = inPackedNormal & ~0x7FF;

	vec3 result;
	result.x = float(x) / float(1 << 11);
	result.y = float(y) / float(1 << 11);
	result.z = float(z) / float(1 << 10);

	return result;
}

void main()
{
	texCoord = vertexTexCoord * 32.0;

	worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	worldNormal = vec3(modelMatrix * vec4(unpackNormal(packedNormal), 1.0));

	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
}