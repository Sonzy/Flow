//= Includes ============================================

#include "Flowpch.h"
#include "Sampler.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "BindableCodex.h"

//= Class (Sampler) Definition ==========================

Sampler::Sampler()
{
	HRESULT ResultHandle;

	D3D11_SAMPLER_DESC SamplerDesc = {};
	//SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //Bilinear
	SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //Anisotropic
	SamplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY; // 16x
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MinLOD = 0.0f;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	SamplerDesc.MipLODBias = 0.0f;

	CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateSamplerState(&SamplerDesc, &m_Sampler));
}

void Sampler::Bind()
{
	RenderCommand::DX11GetContext()->PSSetSamplers(0u, 1u, m_Sampler.GetAddressOf());
}
std::shared_ptr<Bindable> Sampler::Resolve()
{
	return BindableCodex::Resolve<Sampler>();
}
std::string Sampler::GenerateUID()
{
	return typeid(Sampler).name();
}
std::string Sampler::GetUID() const
{
	return GenerateUID();
}