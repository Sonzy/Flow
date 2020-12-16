//Texture2D tex : register(t0);
//SamplerState splr;

//float4 main(float4 Pos : SV_Position, float2 tc : TexCoord, float4 Color : Color) : SV_Target
float4 main(float4 Color : Color) : SV_Target
{
    return float4(Color);
}