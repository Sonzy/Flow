cbuffer CBuf : register (b2)
{
    float4 Colour;
};

Texture2D tex : register(t0);
SamplerState splr;

float4 main() : SV_Target
{
    return Colour;
}