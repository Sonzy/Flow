#include "VertexShaderHelpers.hlsli"

struct VSOut
{
	float3 viewPos : Position;
	float3 n : Normal;
	float2 tex : TexCoord;
	float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tex : TexCoord)
{
	VSOut vso;
	vso.viewPos = (float3) mul(float4(pos, 1.0f), m_modelView);
	vso.tex = tex;
    vso.n = mul(n, (float3x3) m_modelView);
    vso.Pos = mul(float4(pos, 1.0f), m_modelViewProj);
	return vso;
}