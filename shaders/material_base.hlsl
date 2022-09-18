Texture2D base_texture;
SamplerState base_texture_sampler;

cbuffer TransformMatrices
{
	matrix WorldViewProjMatrix;
	matrix WorldMatrix;
};

cbuffer material_base
{
	float3 diffuse;
	float3 selfillum;
	float shininess;
	float shin_strength;
	float3 spec_color;
	float opacity;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float4 WorldPosition : POSITION1;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VSMain(in VS_INPUT input) : SV_POSITION
{
	VS_OUTPUT output;
	output.Position = mul(float4(input.Position, 1.0), WorldViewProjMatrix);
	output.WorldPosition = mul(float4(input.Position, 1.0), WorldMatrix);
	output.TexCoord = input.TexCoord;
	return output;
}

float4 PSMain(in VS_OUTPUT input) : SV_Target
{
	return base_texture.Sample(base_texture_sampler, input.TexCoord);
}