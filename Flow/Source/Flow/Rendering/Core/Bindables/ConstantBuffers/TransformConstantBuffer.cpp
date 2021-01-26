#include "Flowpch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VertexConstBuffer;

TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent, UINT VertexSlot)
	: m_ParentObject(Parent)
{
	if (!m_VertexConstBuffer)
		m_VertexConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
}

void TransformConstantBuffer::Bind()
{
	DirectX::XMMATRIX ParentMatrix;
	if (m_ParentObject)
		ParentMatrix = m_ParentObject->GetTransformXM();
	else
		FLOW_ENGINE_ERROR("TransformConstantBuffer::Bind: Parent was nullptr");

	//Generate the transformation from the parent.
	const auto modelView = 
		RenderCommand::GetActivePass() != RenderPass::Standard2D || RenderCommand::GetActivePass() != RenderPass::UI ?
		ParentMatrix * RenderCommand::GetMainCamera()->GetViewMatrix() :
		ParentMatrix * RenderCommand::GetMainCamera()->GetViewMatrix2D();

	const Transforms transform =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * RenderCommand::GetMainCamera()->GetProjectionMatrix()),
	};

	//Update and bind the constant buffers
	m_VertexConstBuffer->Update(transform);
	m_VertexConstBuffer->Bind();
}