#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"
#include <wrl.h>

namespace Flow
{
	class BindableVertexBuffer : public Bindable
	{
	public:
		BindableVertexBuffer(const VertexBuffer& Buffer);

		void Bind() override;

	protected:
		UINT m_Stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	};
}