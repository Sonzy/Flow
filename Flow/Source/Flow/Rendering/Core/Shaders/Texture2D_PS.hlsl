#include "ShaderHelpers.hlsli"

Texture2D tex : register(t0);
SamplerState splr;

cbuffer IconBuffer
{
    float4 color;
};

float4 main(float3 Pos : Position, float2 tc : TexCoord) : SV_Target
{
    float4 finalColor;
    float4 textureColor = tex.Sample(splr, tc);
    clip(textureColor.a < 0.1f ? -1 : 1); //Give it some tolerance in case of mipmapping    
    
    //TODO: Manage better. Mask it?
    if(textureColor.r > 0.75f && 
       textureColor.g > 0.75f && 
       textureColor.b > 0.75f)
    {
        finalColor = color;
    }
    else
    {
        finalColor = textureColor;
    }
    
    return finalColor;
}