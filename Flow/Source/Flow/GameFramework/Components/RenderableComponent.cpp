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

	if (false && m_useMatrix)
	{
		return m_matrix;
	}

	if (false && m_useQuat)
	{
		Transform WorldTransform = GetWorldTransform();

		return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
			DirectX::XMMatrixRotationQuaternion(m_quat) *
			DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);
	}

	Transform WorldTransform = GetWorldTransform();
	Rotator RadiansRotation = Rotator::AsRadians(WorldTransform.m_Rotation);

	if (bool testQuat = true)
	{
		Rotator RadianRotation = Rotator::AsRadians(WorldTransform.m_Rotation);
		DirectX::XMVECTOR vec = DirectX::XMQuaternionRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll);

		DirectX::XMVECTOR vecQuat = Maths::EulersToQuaternion(RadianRotation);
		DirectX::XMFLOAT4 quat, rpyquat, rpyMathQuat;
		DirectX::XMStoreFloat4(&quat, m_quat);
		DirectX::XMStoreFloat4(&rpyquat, vec);
		DirectX::XMStoreFloat4(&rpyMathQuat, vecQuat);
		if (*(Vector4*)&rpyquat != *(Vector4*)&quat)
		{
			FLOW_ENGINE_LOG("======================");
			FLOW_ENGINE_LOG("XMQuatRollPitchYaw() %f %f %f", RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll);
			Maths::PrintQuaternion((float*)&rpyquat);
			FLOW_ENGINE_LOG("<<<<>>>>");
			Rotator quatrot = Rotator::AsRadians(Maths::QuaternionToEulers(m_quat));
			FLOW_ENGINE_LOG("XMMatrixDecompose()  %f %f %f", quatrot.Pitch, quatrot.Yaw, quatrot.Roll);
			Maths::PrintQuaternion((float*)&quat);
			FLOW_ENGINE_LOG("<<<<>>>>");
			FLOW_ENGINE_LOG("MathsEulerToQuat()  %f %f %f", RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll);
			Maths::PrintQuaternion((float*)&rpyMathQuat);
		}
	}
	
	//THIS WORKS
	return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
		DirectX::XMMatrixRotationQuaternion(Maths::EulersToQuaternion(RadiansRotation)) *
		DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);

	//return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.x, WorldTransform.m_Scale.y, WorldTransform.m_Scale.z) *
	//	DirectX::XMMatrixRotationRollPitchYaw(RadiansRotation.Pitch, RadiansRotation.Yaw, -RadiansRotation.Roll) *
	//	DirectX::XMMatrixTranslation(WorldTransform.m_Position.x, WorldTransform.m_Position.y, WorldTransform.m_Position.z);
}

void RenderableComponent::SetMatrix(DirectX::XMFLOAT4X4 fMatrix)
{
	m_matrix = DirectX::XMLoadFloat4x4(&fMatrix);
	m_useMatrix = true;
}

void RenderableComponent::SetQuat(DirectX::XMVECTOR inQuat)
{
	m_quat = inQuat;
	m_useQuat = true;
}

void RenderableComponent::RefreshBinds()
{
}