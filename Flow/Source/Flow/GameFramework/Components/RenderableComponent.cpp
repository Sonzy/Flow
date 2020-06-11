#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

RenderableComponent::RenderableComponent()
	: WorldComponent("Unnamed RenderableComponent")
{
}

RenderableComponent::RenderableComponent(const std::string& Name)
	: WorldComponent(Name)
{
}

const IndexBuffer& RenderableComponent::GetIndexBuffer() const
{
	return *_IndexBuffer;
}

DirectX::XMMATRIX RenderableComponent::GetTransformXM() const
{
	PROFILE_FUNCTION();

	Transform WorldTransform = GetWorldTransform();
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform._Rotation);
	return 	DirectX::XMMatrixScaling(WorldTransform._Scale.X, WorldTransform._Scale.Y, WorldTransform._Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform._Position.X, WorldTransform._Position.Y, WorldTransform._Position.Z);
}

void RenderableComponent::RefreshBinds()
{
}