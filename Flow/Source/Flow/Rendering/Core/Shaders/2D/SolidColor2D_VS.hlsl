
cbuffer ObjectCBuf
{
    matrix viewProj;
    float2 Position;
    float4 Color;
    float padding;
    float padding2;
};

struct VSOut
{
    float4 Color : Color;
    float2 tex : TexCoord;
    float4 Pos : SV_Position;
};

VSOut main(float2 pos : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.Pos = mul(float4(pos, 0.0f, 0.0f), viewProj);
    vso.tex = tex;
    vso.Color = Color;
    return vso;
}