#pragma once
#include <DirectXMath.h>
#include <memory>
#include "ShaderConstantBuffers.h"

#include "Flow/Rendering/Core/Renderable.h"

namespace Flow
{
	class TransformConstantBuffer : public Bindable
	{
	public:

		TransformConstantBuffer(Renderable* Parent, UINT Slot = 0);
		void Bind() override;

	private:
		struct Transforms
		{
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX modelView;

			DirectX::XMFLOAT3 Scale;
			float padding;
		};

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_VCBuffer;
		Renderable* m_Parent;
		UINT m_Slot;
	};
}