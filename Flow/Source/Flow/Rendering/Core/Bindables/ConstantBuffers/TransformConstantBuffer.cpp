#include "Flowpch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::_VertexConstBuffer;

TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent, UINT VertexSlot)
	: _ParentObject(Parent), _ParentComponent(nullptr)
{
	if (!_VertexConstBuffer)
		_VertexConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
}

TransformConstantBuffer::TransformConstantBuffer(RenderableComponent* Parent, UINT VertexSlot)
	: _ParentObject(nullptr), _ParentComponent(Parent)
{
	if (!_VertexConstBuffer)
		_VertexConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
}

void TransformConstantBuffer::Bind()
{
	DirectX::XMMATRIX ParentMatrix;
	if (_ParentObject)
		ParentMatrix = _ParentObject->GetTransformXM();
	else if (_ParentComponent)
		ParentMatrix = _ParentComponent->GetTransformXM();
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