#include "Flowpch.h"
#include "TransformConstantBuffer.h"
#include "Flow/Rendering/Core/Camera/Camera.h"
#include "Flow\GameFramework\Components\RenderableComponent.h"

#include "Flow\GameFramework\Components\CameraComponent.h"

namespace Flow
{
	std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::VertexConstBuffer_;

	TransformConstantBuffer::TransformConstantBuffer(Renderable* Parent, UINT VertexSlot)
		: ParentObject_(Parent), ParentComponent_(nullptr)
	{
		if (!VertexConstBuffer_)
			VertexConstBuffer_ = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
	}

	TransformConstantBuffer::TransformConstantBuffer(RenderableComponent* Parent, UINT VertexSlot)
		: ParentObject_(nullptr), ParentComponent_(Parent)
	{
		if (!VertexConstBuffer_)
			VertexConstBuffer_ = std::make_unique<VertexConstantBuffer<Transforms>>(VertexSlot);
	}

	void TransformConstantBuffer::Bind()
	{
		DirectX::XMMATRIX ParentMatrix;
		if (ParentObject_)
			ParentMatrix = ParentObject_->GetTransformXM();
		else if (ParentComponent_)
			ParentMatrix = ParentComponent_->GetTransformXM();
		else
			FLOW_ENGINE_ERROR("TransformConstantBuffer::Bind: Parent was nullptr");

		//Generate the transformation from the parent.
		const auto modelView = ParentMatrix * RenderCommand::GetCamera().GetCachedView();
		const Transforms transform =
		{
			DirectX::XMMatrixTranspose(modelView),
			//modelView,
			DirectX::XMMatrixTranspose(
				modelView *
				RenderCommand::GetCamera().GetProjection())
		};

		//Update and bind the constant buffers
		VertexConstBuffer_->Update(transform);
		VertexConstBuffer_->Bind();
	}
}