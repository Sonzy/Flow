float4 main(float4 Position : SV_Position, float3 Color : COLOR) : SV_TARGET
{
    return float4(Color.r, Color.g, Color.b, 1.0f);
}