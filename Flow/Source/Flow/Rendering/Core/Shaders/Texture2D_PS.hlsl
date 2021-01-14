#include "ShaderHelpers.hlsli"

Texture2D tex : register(t0);
SamplerState splr;

float4 main(float3 Pos : Position, float2 tc : TexCoord) : SV_Target
{
    return tex.Sample(splr, tc);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}