#pragma once

// Includes /////////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include "Framework/Logging/Log.h"
#include "Framework/Types/ComPtr.h"
#include "Framework/Utils/DirectX11/DirectX11Utils.h"
#include "Rendering/Core/Bindables/Bindable.h"
#include "Rendering/Core/Bindables/Codex.h"
#include "Rendering/Renderer.h"

//= Class Definitions ///////////////////////////////////////////////////////////////

template<typename C>
class ConstantBuffer : public Bindables::Bindable
{
public:

	// Public Functions /////////////////////////////////////////////////////////////

	ConstantBuffer(const C& consts, UINT slot)
		: ConstantBuffer(consts, slot, "")
	{

	}

	ConstantBuffer(const C& consts, UINT slot, HashString Tag)
		: m_slot(slot)
		, m_tag(Tag)
	{
		CreateResultHandle();

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		CaptureDXError(Renderer::GetDevice()->CreateBuffer(&cbd, &csd, &m_constantBuffer));
	}

	ConstantBuffer(UINT slot)
		: m_slot(slot)
	{
		CreateResultHandle();

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		CaptureDXError(Renderer::GetDevice()->CreateBuffer(&cbd, nullptr, &m_constantBuffer));
	}

	void Update(const C& consts)
	{
		CreateResultHandle();
		D3D11_MAPPED_SUBRESOURCE MSR;

		CaptureDXError(Renderer::GetContext()->Map(
			m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MSR));

		memcpy(MSR.pData, &consts, sizeof(consts));

		Renderer::GetContext()->Unmap(m_constantBuffer.Get(), 0u);
	}

protected:

	//= Protected Variables ///////////////////////////////////////////////////////

	ComPtr<ID3D11Buffer>					m_constantBuffer;
	UINT									m_slot;
	HashString								m_tag;
};