#include "ShaderHelpers.hlsli"

cbuffer CBuf
{
    float4 Colour;
};

Texture2D tex : register(t0);
SamplerState splr;

float4 main(float3 Pos : Position, float2 tc : TexCoord) : SV_Target
{
    return Colour;
}