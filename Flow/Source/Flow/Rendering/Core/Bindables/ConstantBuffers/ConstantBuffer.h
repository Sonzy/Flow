#pragma once
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		
		ConstantBuffer(Graphics& gfx, const C& consts, UINT slot)
			: m_Slot(slot)
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
			//CATCH_ERROR_DX(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &m_ConstantBuffer));
		}

		ConstantBuffer(Graphics& gfx, UINT slot)
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

			//CATCH_ERROR_DX(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &m_ConstantBuffer));
		}

		void Update(const C& consts)
		{
			HRESULT ResultHandle;
			D3D11_MAPPED_SUBRESOURCE MSR;

			//TODO: CATCH_ERROR_DX()
		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		UINT m_Slot;
	};
}