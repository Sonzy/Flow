cbuffer CBuf
{
	matrix model;
	matrix modelViewProj;

	float3 Scale;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{
	//return mul(mul(float4(pos, 1.0f), modelViewProj), float4(1.0f, 1.0f, 1.0f, 1.0f));
	return mul(float4(pos * Scale, 1.0f), modelViewProj);
}