#version 330 core

in vec2 texCoord;

uniform sampler2D TerrainBaseMap;
uniform sampler2D TerrainHeightMap;

out vec4 colour;

void main()
{
	colour = texture(TerrainHeightMap, texCoord);
}