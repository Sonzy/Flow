#include "Flowpch.h"
#include "Sampler.h"
#include "Flow/ErrorHandling/ErrorMacros.h"

namespace Flow
{
	Sampler::Sampler()
	{
		HRESULT ResultHandle;

		D3D11_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateSamplerState(&SamplerDesc, &m_Sampler));
	}

	void Sampler::Bind()
	{
		RenderCommand::DX11GetContext()->PSSetSamplers(0u, 1u, m_Sampler.GetAddressOf());
	}
}