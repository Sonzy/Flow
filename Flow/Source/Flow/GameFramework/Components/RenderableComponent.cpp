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

void RenderableComponent::OnRegistered()
{
	WorldComponent::OnRegistered();

	uint32 guid = GetGuid();
	m_SelectionConstantBuffer.m_guidColor = Vector4(
		(float)(guid & 0xff000000) / 4278190080.0f,
		(float)(guid & 0x00ff0000) / 16711680.0f,
		(float)(guid & 0x0000ff00) / 65280.0f,
		(float)(guid & 0x000000ff) / 255.0f);

	FLOW_ENGINE_LOG("GUID: %lu", guid);
	FLOW_ENGINE_LOG("GUID: %lu, %lu, %lu, %lu", guid & 0xff000000, guid & 0x00ff0000, guid & 0x0000ff00, guid & 0x000000ff);
	FLOW_ENGINE_LOG("CBUFFER: %f %f %f %f", m_SelectionConstantBuffer.m_guidColor.x, m_SelectionConstantBuffer.m_guidColor.y, m_SelectionConstantBuffer.m_guidColor.z, m_SelectionConstantBuffer.m_guidColor.w);
	FLOW_ENGINE_LOG("/////////////////////////////////////////////////////////////////////////", guid);
}

void RenderableComponent::RefreshBinds()
{
}