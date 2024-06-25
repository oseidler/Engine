#pragma once


//HLSL shader source code
const char* g_defaultShaderSource = R"(
cbuffer CameraConstants : register(b2)
{
	float4x4 ProjectionMatrix;
	float4x4 ViewMatrix;
};

cbuffer ModelConstants : register(b3)
{
	float4x4 ModelMatrix;
	float4 ModelColor;
};

SamplerState samplerSlot : register(s0);

Texture2D textureSlot : register(t0);

struct vs_input_t
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct v2p_t
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

v2p_t VertexMain(vs_input_t input)
{
	float4 worldPosition = mul(ModelMatrix, float4(input.localPosition, 1));
	
	float4 viewPosition = mul(ViewMatrix, worldPosition);

	float4 clipPosition = mul(ProjectionMatrix, viewPosition);

	v2p_t v2p;
	v2p.position = clipPosition;
	v2p.color = input.color * ModelColor;
	v2p.uv = input.uv;
	return v2p;
}

float4 PixelMain(v2p_t input) : SV_Target0
{
	float4 finalColor = textureSlot.Sample(samplerSlot, input.uv) * float4(input.color);
	clip(finalColor.a - 0.01f);

	return finalColor;
};
)";