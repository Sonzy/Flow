cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
}

struct VSOut
{
    float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tex : TexCoord)
{
    VSOut vso;
    vso.Pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}