#include "Flowpch.h"
#include "TransformConstantBuffer.h"

namespace Flow
{
	Flow::TransformConstantBuffer::TransformConstantBuffer(const Renderable& Parent, UINT Slot)
		: m_Parent(Parent), m_Slot(Slot)
	{
		if (!m_VCBuffer)
			m_VCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(Slot);
	}

	void Flow::TransformConstantBuffer::Bind()
	{
		//TODO 
		//const auto modelView = m_Parent.GetTransformXM();*gfx.GetCamera();
		//const Transforms tf =
		//{
		//	DirectX::XMMatrixTranspose(modelView),
		//	DirectX::XMMatrixTranspose(
		//		modelView *
		//		gfx.GetProjection()
		//	)
		//};
		//vcBuffer->Update(gfx, tf);
		//vcBuffer->Bind(gfx);
	}
}