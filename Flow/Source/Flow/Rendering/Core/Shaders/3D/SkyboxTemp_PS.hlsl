//#include "Flow/Rendering/Core/Shaders/ShaderHelpers.hlsli"
#include <ShaderHelpers3D.hlsli>

cbuffer ObjectCBuf
{
	float3 specularColor;
	float specularWeight;
	float specularGloss;
};

Texture2D tex : register(t0);
SamplerState splr;

float4 main(float3 Pos : Position, float3 n : Normal, float2 tc : TexCoord) : SV_Target
{
	// normalize the mesh normal
	n = normalize(n);
	// fragment to light vector data
	const LightVectorData lv = CalculateLightVectorData(LightPos, Pos);
	// attenuation
	const float att = Attenuate(AttenuationConstant, AttenuationLinear, AttenuationQuadratic, lv.distToL);
	// diffuse
	const float3 diffuse = Diffuse(DiffuseColour, DiffuseIntensity, att, lv.dirToL, n);
	// specular
	const float3 specular = Speculate(
		DiffuseColour * DiffuseIntensity * specularColor, specularWeight, n,
		lv.vToL, Pos, att, specularGloss);
	// final color
	return float4(saturate((diffuse + Ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}