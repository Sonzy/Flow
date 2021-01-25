#include "Flowpch.h"
#include <iostream>
#include "Maths/Vector3.h"
#include "PointLightComponent.h"
#include "ThirdParty/ImGui/imgui.h"

#if WITH_EDITOR
	#include "Editor/Editor.h"
	#include "Editor/IconManager.h"
#endif


#include <yaml-cpp/yaml.h>
#include "Utils/YamlSerializer.h"

//TODO: Support multiple lights

PointLightComponent::PointLightComponent()
	: RenderableComponent("Unnamed Point Light Component")
	, m_lightPixelBuffer(0)
{
}

PointLightComponent::PointLightComponent(const std::string& ComponentName)
	: RenderableComponent(ComponentName)
	, m_lightPixelBuffer(0)
{
	m_lightBuffer = {
	{ 1.5f,100.0f,-4.5f },
	{ 0.05f,0.05f,0.05f },
	{ 1.0f,1.0f,1.0f },
	1.0f,
	1.0f,
	0.045f,
	0.0075f,
	};
}

void PointLightComponent::OnRegistered()
{
#if WITH_EDITOR
	IconManager::IconData iconData;
	iconData.m_texture = nullptr;
	Editor::Get().GetUIComponent<IconManager>()->RegisterIcon(GetGuid(), iconData);
#endif
}

void PointLightComponent::Render()
{
	//Update from component position
	m_lightBuffer.m_Position = GetWorldPosition();

	//Create a copy and transform the copied position with the view matrix
	LightBuffer_t Copy = m_lightBuffer;
	const DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&m_lightBuffer.m_Position);

	//TODO: Might have to recalculate
	DirectX::XMStoreFloat3(&Copy.m_Position, DirectX::XMVector3Transform(Position, RenderCommand::GetMainCamera()->GetCachedView()));// modelView

	//Update the transformed position to the shader
	m_lightPixelBuffer.Update(Copy);
	m_lightPixelBuffer.Bind();

	IconManager* iconManager = Editor::Get().GetUIComponent<IconManager>();

	Vector3 screen = RenderCommand::WorldToScreen(GetWorldPosition());
	Icon& lightIcon = iconManager->GetIcon(GetGuid());
	lightIcon.m_position.x = screen.x;
	lightIcon.m_position.y = screen.y;
	lightIcon.m_tint = Vector4(m_lightBuffer.m_Diffuse.x, m_lightBuffer.m_Diffuse.y, m_lightBuffer.m_Diffuse.z, 1.0f);
	lightIcon.m_alignment = Icon::Alignment::Centre;
	lightIcon.RefreshBinds(*iconManager);
}

void PointLightComponent::DrawComponentDetailsWindow()
{
	ImGui::TextColored(IMGUI_YELLOW, "Point Light Settings");
	ImGui::ColorPicker3("Light Diffuse Colour", reinterpret_cast<float*>(&m_lightBuffer.m_Diffuse));		
	ImGui::ColorPicker3("Ambient Colour", reinterpret_cast<float*>(&m_lightBuffer.m_Ambient));
	ImGui::InputFloat("Diffuse Intensity", reinterpret_cast<float*>(&m_lightBuffer.m_DiffuseIntensity));				
	ImGui::InputFloat("Attenuation Constant", reinterpret_cast<float*>(&m_lightBuffer.m_AttenuationConstant));				
	ImGui::InputFloat("Attenuation Linear", reinterpret_cast<float*>(&m_lightBuffer.m_AttenuationLinear));				
	ImGui::InputFloat("Attenuation Quadratic", reinterpret_cast<float*>(&m_lightBuffer.m_AttenuationQuadratic));				
}

void PointLightComponent::Serialize(YAML::Emitter& Archive)
{
	WorldComponent::Serialize(Archive);

	Archive << YAML::Key << "PointLightComponent";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "Position";
		Archive << YAML::Value << static_cast<Vector3>(m_lightBuffer.m_Position);

		Archive << YAML::Key << "Ambient";
		Archive << YAML::Value << static_cast<Vector3>(m_lightBuffer.m_Ambient);

		Archive << YAML::Key << "Diffuse";
		Archive << YAML::Value << static_cast<Vector3>(m_lightBuffer.m_Diffuse);

		Archive << YAML::Key << "DiffuseIntensity";
		Archive << YAML::Value << m_lightBuffer.m_DiffuseIntensity;

		Archive << YAML::Key << "AttenuationConstant";
		Archive << YAML::Value << m_lightBuffer.m_AttenuationConstant;

		Archive << YAML::Key << "AttenuationLinear";
		Archive << YAML::Value << m_lightBuffer.m_AttenuationLinear;

		Archive << YAML::Key << "AttenuationQuadratic";
		Archive << YAML::Value << m_lightBuffer.m_AttenuationQuadratic;
	}
	Archive << YAML::EndMap;
}

void PointLightComponent::Deserialize(YAML::Node& Archive)
{
	WorldComponent::Deserialize(Archive);

	if (YAML::Node node = Archive["PointLightComponent"])
	{
		m_lightBuffer.m_Position = node["Position"].as<Vector3>();
		m_lightBuffer.m_Ambient = node["Ambient"].as<Vector3>();
		m_lightBuffer.m_Diffuse = node["Diffuse"].as<Vector3>();
		m_lightBuffer.m_DiffuseIntensity = node["DiffuseIntensity"].as<float>();
		m_lightBuffer.m_AttenuationConstant = node["AttenuationConstant"].as<float>();
		m_lightBuffer.m_AttenuationLinear = node["AttenuationLinear"].as<float>();
		m_lightBuffer.m_AttenuationQuadratic = node["AttenuationQuadratic"].as<float>();
	}
}