#include "Flowpch.h"
#include "ScaledTransformConstantBuffer.h"

ScaledTransformConstantBuffer::ScaledTransformConstantBuffer(Renderable* Parent, UINT VertexSlot)
	: TransformConstantBuffer(Parent, VertexSlot)
{
}

void ScaledTransformConstantBuffer::Bind()
{
	DirectX::XMMATRIX ParentMatrix;
	if (_ParentObject)
		ParentMatrix = DirectX::XMMatrixScaling(1.05f, 1.05f, 1.05f) * _ParentObject->GetTransformXM();
	else
		FLOW_ENGINE_ERROR("TransformConstantBuffer::Bind: Parent was nullptr");

	//Generate the transformation from the parent.
	const auto modelView = ParentMatrix * RenderCommand::GetMainCamera()->GetViewMatrix();
	const Transforms transform =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			RenderCommand::GetMainCamera()->GetProjectionMatrix())
	};

	//Update and bind the constant buffers
	_VertexConstBuffer->Update(transform);
	_VertexConstBuffer->Bind();
}
