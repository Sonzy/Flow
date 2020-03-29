cbuffer CBuf : register(b3)
{
	float4 Colour;
};

float4 main() : SV_TARGET
{
	return Colour;
}