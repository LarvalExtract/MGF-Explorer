#version 400 core

in vec2 texCoord;

uniform sampler2D TerrainBaseMap;
uniform sampler2D TerrainDetailMap0;
uniform sampler2D TerrainDetailMap1;
uniform sampler2D TerrainMicroDetailMap0;
uniform sampler2D TerrainMicroDetailMap1;

uniform float TerrainDetailBlend;
uniform float TerrainDetailMultiplier0;
uniform float TerrainDetailMultiplier1;
uniform float TerrainMicroDetailTiling0;
uniform float TerrainMicroDetailTiling1;

out vec4 colour;

void main()
{
	vec4 baseColour = texture(TerrainBaseMap, texCoord);
	vec4 detail0 = texture(TerrainDetailMap0, texCoord * TerrainDetailMultiplier0);
	vec4 detail1 = texture(TerrainDetailMap1, texCoord * TerrainDetailMultiplier1);
	//vec4 microDetail0 = texture(TerrainMicroDetailMap0, texCoord * TerrainMicroDetailTiling0);
	//vec4 microDetail1 = texture(TerrainMicroDetailMap1, texCoord * TerrainMicroDetailTiling1);

	colour = baseColour * mix(detail0, detail1, baseColour.w) * TerrainDetailBlend * 4.0;
}