cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
}

struct VSOut
{
    float3 viewPos : Position;
    float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.viewPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.Pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}