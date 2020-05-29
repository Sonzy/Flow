#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include "Flow\Rendering\Core\Bindables\BindableCodex.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:

	ConstantBuffer(const C& consts, UINT slot)
		: ConstantBuffer(consts, slot, "")
	{

	}

	ConstantBuffer(const C& consts, UINT slot, const std::string& Tag)
		: _Slot(slot), _Tag(Tag)
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
		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&cbd, &csd, &_ConstantBuffer));
	}

	ConstantBuffer(UINT slot)
		: _Slot(slot)
	{
		HRESULT ResultHandle;

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateBuffer(&cbd, nullptr, &_ConstantBuffer));
	}

	void Update(const C& consts)
	{
		HRESULT ResultHandle;
		D3D11_MAPPED_SUBRESOURCE MSR;

		CATCH_ERROR_DX(RenderCommand::DX11GetContext()->Map(
			_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &MSR));

		memcpy(MSR.pData, &consts, sizeof(consts));

		RenderCommand::DX11GetContext()->Unmap(_ConstantBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _ConstantBuffer;
	UINT _Slot;
	std::string _Tag;
};