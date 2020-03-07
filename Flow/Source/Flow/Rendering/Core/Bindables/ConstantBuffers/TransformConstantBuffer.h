#pragma once
#include <DirectXMath.h>
#include <memory>
#include "ShaderConstantBuffers.h"

#include "Flow/Rendering/Core/Renderable.h"

namespace Flow
{
	class RenderableComponent;

	class TransformConstantBuffer : public Bindable
	{
	public:

		TransformConstantBuffer(Renderable* Parent, UINT VertexSlot = 0);
		TransformConstantBuffer(RenderableComponent* Parent, UINT VertexSlot = 0);
		void Bind() override;

	private:
		struct Transforms
		{
			DirectX::XMMATRIX modelView;
			DirectX::XMMATRIX modelViewProj;
		};

	private:
		std::string m_Tag;
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_VCBuffer;
		Renderable* m_Parent;
		RenderableComponent* m_ComponentParent;
	};
}