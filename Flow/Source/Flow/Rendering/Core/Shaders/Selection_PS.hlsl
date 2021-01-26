cbuffer test : register(b7)
{
    float colorR;
    float colorG;
    float colorB;
    float colorA;
}

float4 main() : SV_TARGET
{
    return float4(colorR, colorG, colorB, colorA);
}