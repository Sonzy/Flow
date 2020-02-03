#include "Flowpch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"

namespace Flow
{
	std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VCBuffer;

	TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent, UINT Slot)
		: m_Parent(Parent), m_Slot(Slot)
	{
		if (!m_VCBuffer)
			m_VCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(Slot);
	}

	void TransformConstantBuffer::Bind()
	{
		CHECK_RETURN(!m_Parent, "TransformConstantBuffer::Bind: Parent was nullptr");

		const auto modelView = m_Parent->GetTransformXM() * RenderCommand::GetCamera().GetMatrix();
		const Transforms tf =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				RenderCommand::GetCamera().GetProjection()),
			m_Parent->GetScale().ToDXFloat3()
		};
		m_VCBuffer->Update(tf);
		m_VCBuffer->Bind();
	}
}