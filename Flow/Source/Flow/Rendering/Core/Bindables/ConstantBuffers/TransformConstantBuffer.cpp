#include "Flowpch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

namespace Flow
{
	std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VCBuffer;

	TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent, UINT VertexSlot)
		: m_Parent(Parent), m_ComponentParent(nullptr)
	{
		if (!m_VCBuffer)
			m_VCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
	}

	TransformConstantBuffer::TransformConstantBuffer(RenderableComponent* Parent, UINT VertexSlot)
		: m_Parent(nullptr), m_ComponentParent(Parent)
	{
		if (!m_VCBuffer)
			m_VCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
	}

	void TransformConstantBuffer::Bind()
	{
		DirectX::XMMATRIX ParentMatrix;
		if (m_Parent)
			ParentMatrix = m_Parent->GetTransformXM();
		else if (m_ComponentParent)
			ParentMatrix = m_ComponentParent->GetTransformXM();
		else
			FLOW_ENGINE_ERROR("TransformConstantBuffer::Bind: Parent was nullptr");

		//Generate the transformation from the parent.
		const auto modelView = ParentMatrix * RenderCommand::GetCamera().GetMatrix();
		const Transforms transform =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				RenderCommand::GetCamera().GetProjection())
		};

		//Update and bind the constant buffers
		m_VCBuffer->Update(transform);
		m_VCBuffer->Bind();
	}
}