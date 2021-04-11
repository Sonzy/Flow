#include "pch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VertexConstBuffer;

TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent)
	: m_ParentObject(Parent)
{
	if (!m_VertexConstBuffer)
		m_VertexConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(0);
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
		Renderer::GetActivePass() != RenderPass::Standard2D || Renderer::GetActivePass() != RenderPass::UI ?
		ParentMatrix * Renderer::GetMainCamera()->GetViewMatrix() :
		ParentMatrix * Renderer::GetMainCamera()->GetViewMatrix2D();

	const Transforms transform =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(modelView * Renderer::GetMainCamera()->GetProjectionMatrix()),
	};

	//Update and bind the constant buffers
	m_VertexConstBuffer->Update(transform);
	m_VertexConstBuffer->Bind();
}