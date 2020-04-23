cbuffer CBuf
{
    matrix ViewProj;
}

struct VSOut
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

VSOut main(float3 pos : POSITION, float3 color : COLOR)
{
    VSOut vs;
    vs.Position = mul(float4(pos, 1.0f), ViewProj);
    vs.Color = color;
    return vs;
}