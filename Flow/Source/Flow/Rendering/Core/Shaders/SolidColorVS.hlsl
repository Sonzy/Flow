//cbuffer CBuf
//{
//	matrix model;
//	matrix modelViewProj;
//
//	float3 Scale;
//}
//
//float4 main(float3 pos : POSITION) : SV_POSITION
//{
//	//return mul(mul(float4(pos, 1.0f), modelViewProj), float4(1.0f, 1.0f, 1.0f, 1.0f));
//	return mul(float4(pos * Scale, 1.0f), modelViewProj);
//}

cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
}

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
    vso.viewPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.tex = tex;
    vso.n = mul(n, (float3x3) modelView);
    vso.Pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}