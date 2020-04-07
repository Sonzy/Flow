#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

namespace Flow
{
	RenderableComponent::RenderableComponent()
		: WorldComponent("Unnamed RenderableComponent"), IndexBuffer_(nullptr)
	{
	}

	RenderableComponent::RenderableComponent(const std::string& Name)
		: WorldComponent(Name), IndexBuffer_(nullptr)
	{
	}

	void RenderableComponent::BindAll()
	{
		for (auto& b : Binds_)
		{
			b->Bind();
		}
	}

	const IndexBuffer& RenderableComponent::GetIndexBuffer() const
	{
		return *IndexBuffer_;
	}

	DirectX::XMMATRIX RenderableComponent::GetTransformXM()
	{
		Transform WorldTransform = GetWorldTransform();
		Rotator RadianRotation = Rotator::AsRadians(WorldTransform.Rotation_);
		return 	DirectX::XMMatrixScaling(WorldTransform.Scale_.X, WorldTransform.Scale_.Y, WorldTransform.Scale_.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) * 
			DirectX::XMMatrixTranslation(WorldTransform.Position_.X, WorldTransform.Position_.Y, WorldTransform.Position_.Z);
	}

	void RenderableComponent::AddBind(std::shared_ptr<Bindable> bind)
	{
		//If index buffer, only allow single bind.
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Renderable::AddBind: Cannot bind multiple index buffers." && IndexBuffer_ == nullptr);
			IndexBuffer_ = static_cast<IndexBuffer*>(bind.get());
		}

		Binds_.push_back(std::move(bind));
	}
	void RenderableComponent::RefreshBinds()
	{
	}
}