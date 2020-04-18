Texture2D Texture;
SamplerState Sampler;

float4 main(float4 Pos : SV_POSITION, float2 TexCoord : Texcoord) : SV_TARGET
{
	return float4(Texture.Sample(Sampler, TexCoord).rgb, 1.0f);
}