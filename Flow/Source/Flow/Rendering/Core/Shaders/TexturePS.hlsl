Texture2D tex : register(t0);
SamplerState splr;

float4 main(float2 tc : TexCoord) : SV_Target
{
    return 
    //float4(1.0f, 1.0f, 1.0f, 1.0f);
    tex.Sample(splr, tc);

}