cbuffer Orth : register(b3)
{
	matrix WorldView;
};

struct VSOut
{
	float4 OutPos : SV_POSITION;
	float2 OutTexCoord : Texcoord;
};

VSOut main(float3 Pos : POSITION, float2 TexCoord : Texcoord)
{
	VSOut Out;
	Out.OutPos = mul(float4(Pos, 1.0f), WorldView);
	Out.OutTexCoord = TexCoord;
	return Out;
}