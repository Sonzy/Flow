#include "ShaderHelpers.hlsli"

cbuffer CBuf : register(b0)
{
    float4 color;
};

float4 main(float3 Pos : Position, float3 n : Normal) : SV_TARGET
{
    float3 normal = normalize(n);
    const float3 lightDirection = normalize(m_directional_lightDirection);
       
	// Directional light
    
    float3 directional_output = CalculateDirectionalLight(Pos, normal, lightDirection);
    float3 point_output = CalculatePointLight(Pos, normal);
    
	// final color
    return float4(directional_output + point_output, 1.0f) * color;
}