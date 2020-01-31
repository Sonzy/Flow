#include "Sampler.h"

namespace Flow
{
	Sampler::Sampler()
	{
		HRESULT ResultHandle;

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		//TODO: GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind()
	{
		//TODO: GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
	}
}