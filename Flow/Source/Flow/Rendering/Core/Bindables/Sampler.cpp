// Pch ////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Includes ///////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include "Sampler.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

// Class Definition ///////////////////////////////////////////////////////////

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

	CaptureDXError(Renderer::GetDevice()->CreateSamplerState(&SamplerDesc, &m_sampler));
}

void Sampler::Bind()
{
	Renderer::GetContext()->PSSetSamplers(0u, 1u, m_sampler.GetAddressOf());
}

Sampler* Sampler::Resolve()
{
	return Bindables::Codex::Resolve<Sampler>();
}

HashString Sampler::GenerateID()
{
	return "Sampler";
}

HashString Sampler::GetID() 
{
	if (m_id.IsNull())
	{
		m_id = GenerateID();
	}
	return m_id;
}