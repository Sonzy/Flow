#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "Flow/Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include <DirectXMath.h>
#include <memory>

namespace Flow
{


	class TransformConstantBuffer : public Bindable
	{
	public:

		TransformConstantBuffer(const Renderable& Parent, UINT Slot = 0);
		void Bind() override;

	private:
		struct Transforms
		{
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX modelView;
		};

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_VCBuffer;
		const Renderable& m_Parent;
		UINT m_Slot;
	};
}