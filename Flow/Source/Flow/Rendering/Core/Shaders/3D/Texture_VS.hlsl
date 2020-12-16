#include "ShaderHelpers3D.hlsli"

struct VSOut
{
    float2 tex : TexCoord;
    float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tex : TexCoord)
{
    VSOut vso;
    vso.tex = tex;
    vso.Pos = mul(float4(pos, 1.0f), modelViewProj);
    float dummy = n.x;
    return vso;
}