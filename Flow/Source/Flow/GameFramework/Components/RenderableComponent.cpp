#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

RenderableComponent::RenderableComponent()
	: WorldComponent("Unnamed RenderableComponent")
	, m_useMatrix(false)
{
}

RenderableComponent::RenderableComponent(const std::string& Name)
	: WorldComponent(Name)
{
}

const IndexBuffer& RenderableComponent::GetIndexBuffer() const
{
	return *m_IndexBuffer;
}

DirectX::XMMATRIX RenderableComponent::GetTransformXM() const
{
	PROFILE_FUNCTION();

	//if (m_useMatrix)
	//{
	//	return m_matrix;
	//}

	Transform WorldTransform = GetWorldTransform();
	Rotator RadianRotation = Rotator::AsRadians(WorldTransform.m_Rotation);
	return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) *
		DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);
}

void RenderableComponent::SetMatrix(DirectX::XMFLOAT4X4 fMatrix)
{
	m_matrix = DirectX::XMLoadFloat4x4(&fMatrix);
	m_useMatrix = true;
}

void RenderableComponent::RefreshBinds()
{
}