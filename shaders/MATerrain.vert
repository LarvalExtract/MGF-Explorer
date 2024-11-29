#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat3 modelNormalMatrix;
uniform mat4 modelViewProjection;

uniform sampler2D TerrainHeightMap;
uniform float maxHeight;
uniform float minHeight;

void main()
{
	float heightValue = textureLod(TerrainHeightMap, vertexTexCoord, 0).x;
	float mapped = heightValue * ((maxHeight - minHeight) + minHeight);

	texCoord = vertexTexCoord;

	worldPosition = vec3(modelMatrix * vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0));
	worldPosition.y += heightValue * 8;
	//worldNormal = normalize(modelNormalMatrix);
	gl_Position = modelViewProjection * vec4(worldPosition, 1.0);
}