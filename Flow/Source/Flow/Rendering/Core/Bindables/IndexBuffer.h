#pragma once
#include <vector>
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(const std::vector<unsigned short>& Indices);

		virtual void Bind() override;

		UINT GetCount() const;

	public:

		UINT m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}