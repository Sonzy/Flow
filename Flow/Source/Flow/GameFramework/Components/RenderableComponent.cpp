#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

RenderableComponent::RenderableComponent()
	: WorldComponent("Unnamed RenderableComponent"), _IndexBuffer(nullptr)
{
}

RenderableComponent::RenderableComponent(const std::string& Name)
	: WorldComponent(Name), _IndexBuffer(nullptr)
{
}

void RenderableComponent::BindAll()
{
	for (auto& b : _Binds)
	{
		b->Bind();
	}
}

const IndexBuffer& RenderableComponent::GetIndexBuffer() const
{
	return *_IndexBuffer;
}

DirectX::XMMATRIX RenderableComponent::GetTransformXM()
{
	Transform WorldTransform = GetWorldTransform();
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform._Rotation);
	return 	DirectX::XMMatrixScaling(WorldTransform._Scale.X, WorldTransform._Scale.Y, WorldTransform._Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform._Position.X, WorldTransform._Position.Y, WorldTransform._Position.Z);
}

void RenderableComponent::AddBind(std::shared_ptr<Bindable> bind)
{
	//If index buffer, only allow single bind.
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Renderable::AddBind: Cannot bind multiple index buffers." && _IndexBuffer == nullptr);
		_IndexBuffer = static_cast<IndexBuffer*>(bind.get());
	}

	_Binds.push_back(std::move(bind));
}
void RenderableComponent::RefreshBinds()
{
}