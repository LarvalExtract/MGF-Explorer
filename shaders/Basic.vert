#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in uint packedNormal;
in vec2 vertexTexCoord;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat3 modelNormalMatrix;
uniform mat4 modelViewProjection;

vec3 unpackNormal(uint inPackedNormal)
{
	uint x = inPackedNormal >> 21u;
	uint y = inPackedNormal >> 11u & 0x7FFu;
	uint z = inPackedNormal & 0xFFC0u;

	float fx = float(x) / float(1u << 11u);
	float fy = float(y) / float(1u << 11u);
	float fz = float(z) / float(1u << 10u);
	
	fx = fx * 2.0 - 1.0;
	fy = fy * 2.0 - 1.0;
	fz = fz * 2.0 - 1.0;
	
	return normalize(vec3(fx, fy, fz));
}

void main()
{
	texCoord = vertexTexCoord * 32.0;

	worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	worldNormal = normalize(modelNormalMatrix * unpackNormal(packedNormal));
	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);
}