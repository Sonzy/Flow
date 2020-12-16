#include "ShaderHelpers3D.hlsli"

float4 main(float3 pos : Position, float2 tex : TexCoord) : SV_Position
{
    return mul(float4(pos, 1.0f), modelViewProj);
}