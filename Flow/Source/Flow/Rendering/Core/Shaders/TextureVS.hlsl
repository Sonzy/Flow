cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;

    float3 Scale;
}

struct VSOut
{
    float2 tex : TexCoord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.pos = mul(float4(pos * Scale, 1.0f), modelView);
    vso.tex = tex;
    return vso;
}