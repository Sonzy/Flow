#include "ShaderHelpers.hlsli"

cbuffer ObjectCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

cbuffer DirLightCBuffer : register(b1)
{
    float3 Direction;
    float padding;
    float3 RotatedDir;
    float padding2;
    float4 AmbientColour;
    float4 LightColour;
};

cbuffer CBuf : register(b2)
{
    float4 Colour;
};

float4 main(float3 Pos : Position, float3 n : Normal, float2 tc : TexCoord, float4 VPPos : SV_Position, matrix m : Multiply) : SV_Target
{
    n = normalize(n);
    
    float3 PhongLightDirection = mul(Direction, (float3x3) m); //Rotate light direction with world
    
    float4 ObjectDiffuse = Colour;        
    float4 FinalColour = ObjectDiffuse * AmbientColour;
    FinalColour += float4(saturate(max(0.0f, dot(PhongLightDirection, n)) * LightColour.rgb * ObjectDiffuse.rgb), 1.0f);
    
    return FinalColour;
}