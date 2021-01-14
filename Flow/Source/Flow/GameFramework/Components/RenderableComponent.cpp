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
	return *m_IndexBuffer;
}

DirectX::XMMATRIX RenderableComponent::GetTransformXM() const
{
	PROFILE_FUNCTION();

	Transform WorldTransform = GetWorldTransform();
	Rotator RadiansRotation = Rotator::AsRadians(WorldTransform.m_Rotation);

	if (const bool useNormal = false)
	{
		return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
			DirectX::XMMatrixRotationQuaternion(Maths::EulersToQuaternion(RadiansRotation)) * 
			//TODO: XMMAtrixRotationRollPitchYaw doesn't work here. Not sure what is different between the conversion formulae
			DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);
	}
	else
	{
		return 
			DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z) *
			DirectX::XMMatrixRotationQuaternion(Maths::EulersToQuaternion(RadiansRotation)) *
			DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z);
	}
}

void RenderableComponent::RefreshBinds()
{
}