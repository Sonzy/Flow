#include "ShaderHelpers.hlsli"

cbuffer ObjectCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

cbuffer CBuf : register(b2)
{
    float4 Colour;
};

Texture2D tex : register(t0);
SamplerState splr;

float4 main(float3 Pos : Position, float2 tc : TexCoord) : SV_Target
{
    return Colour;
}