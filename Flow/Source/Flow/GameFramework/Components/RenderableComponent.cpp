#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

#include "Assets/AssetSystem.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Rendering/Core/Bindables/Shaders/PixelShader.h"
#include "Rendering/Core/Bindables/Shaders/VertexShader.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"

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
}

void RenderableComponent::RefreshBinds()
{
	m_Techniques.clear();

	if (IsRegistered() == true)
	{
		Technique Selection = Technique("RenderableComponent_Selection");
		Step Rendering(RenderPass::Selection);

		//No need to recreate mesh binds

		std::string tag = "SelectionBuffer_" + std::to_string(GetGuid());
		Rendering.AddBindable(PixelConstantBuffer<SelectionPassConstantBuffer>::Resolve(m_SelectionConstantBuffer, 7, tag));

		auto vShader = VertexShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("Selection_VS")->GetPath());
		auto vShaderByteCode = static_cast<VertexShader&>(*vShader).GetByteCode();
		Rendering.AddBindable(std::move(vShader));
		Rendering.AddBindable(PixelShader::Resolve(AssetSystem::GetAsset<ShaderAsset>("Selection_PS")->GetPath()));

		Rendering.AddBindable(new TransformConstantBuffer(this));

		Selection.AddStep(std::move(Rendering));

		AddTechnique(Selection);
	}
}