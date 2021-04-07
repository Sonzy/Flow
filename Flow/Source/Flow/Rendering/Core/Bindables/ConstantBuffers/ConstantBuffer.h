#pragma once

//= Includes =====================================================

#include "Framework/Logging/Log.h"
#include "Rendering/Core/Bindable.h"
#include "Rendering/Core/Bindables/BindableCodex.h"

//= Class Definitions =============================================

template<typename C>
class ConstantBuffer : public Bindable
{
public:

	//= Public Functions ==============================================================

	ConstantBuffer(const C& consts, UINT slot)
		: ConstantBuffer(consts, slot, "")
	{

	}

	ConstantBuffer(const C& consts, UINT slot, const std::string& Tag)
		: m_Slot(slot)
		, m_Tag(Tag)
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		CaptureDXError(RenderCommand::DX11GetDevice()->CreateBuffer(&cbd, &csd, &m_ConstantBuffer));
	}

	ConstantBuffer(UINT slot)
		: m_Slot(slot)
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		CaptureDXError(RenderCommand::DX11GetDevice()->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer));
	}

	void Update(const C& consts)
	{
		HRESULT ResultHandle;
		D3D11_MAPPED_SUBRESOURCE MSR;

		CaptureDXError(RenderCommand::DX11GetContext()->Map(
			m_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MSR));

		memcpy(MSR.pData, &consts, sizeof(consts));

		RenderCommand::DX11GetContext()->Unmap(m_ConstantBuffer.Get(), 0u);
	}

protected:

	//= Protected Variables ===========================================================
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_ConstantBuffer;
	UINT									m_Slot;
	std::string								m_Tag;
};