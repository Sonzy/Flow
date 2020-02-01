#include "Flowpch.h"
#include "TransformConstantBuffer.h"


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

		const auto modelView = m_Parent->GetTransformXM();//TODO: Get Camera location *gfx.GetCamera();
		const Transforms tf =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView * 
				DirectX::XMMATRIX()//TODO: Get Camera Projection *gfx.GetCamera();gfx.GetProjection()
			)
		};
		m_VCBuffer->Update(tf);
		m_VCBuffer->Bind();
	}
}