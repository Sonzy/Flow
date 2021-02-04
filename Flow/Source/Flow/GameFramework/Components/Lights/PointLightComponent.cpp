#include "Flowpch.h"
#include <iostream>
#include "Maths/Vector3.h"
#include "PointLightComponent.h"
#include "ThirdParty/ImGui/imgui.h"

#include "Assets/Materials/MaterialCommon.h"

#include "Rendering/Core/Camera/Camera.h"

#if WITH_EDITOR
#include "Assets/AssetSystem.h"
	#include "Editor/Editor.h"
	#include "Editor/IconManager.h"
#endif


#include <yaml-cpp/yaml.h>
#include "Utils/YamlSerializer.h"

//TODO: Support multiple lights

PointLightComponent::PointLightComponent()
	: PointLightComponent("Point Light Component")
{
}

PointLightComponent::PointLightComponent(const std::string& ComponentName)
	: RenderableComponent(ComponentName)
	, m_lightPixelBuffer(MaterialCommon::Register::PointLightProperties)
{
	m_lightBuffer.m_Diffuse	= { 1.0f, 1.0f, 1.0f };
	m_lightBuffer.m_Position = { 0.0f, 0.0f, 0.0f };
	m_lightBuffer.m_DiffuseIntensity = 1.0f;
	m_lightBuffer.m_specularIntensity = 1.0f;	
	m_lightBuffer.m_attenuationConstant = 1.0f;	
	m_lightBuffer.m_attenuationLinear = 0.09f;	
	m_lightBuffer.m_attenuationQuadratic = 0.032f;	
}

void PointLightComponent::OnRegistered()
{
#if WITH_EDITOR
	IconManager::IconData iconData;
	iconData.m_texture = AssetSystem::GetAsset<TextureAsset>("Icon_Light");
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
}

void PointLightComponent::DrawComponentDetailsWindow()
{
	ImGui::TextColored(IMGUI_YELLOW, "Point Light Settings");
	ImGui::ColorEdit3("Light Diffuse Colour", reinterpret_cast<float*>(&m_lightBuffer.m_Diffuse));

	float width = ImGui::GetContentRegionAvailWidth();
	ImGui::Columns(3, "ComponentDeets", true);

	ImGui::SetColumnWidth(0, width * 0.4f);
	ImGui::SetColumnWidth(1, width * 0.4f);
	ImGui::SetColumnWidth(2, width * 0.2f);

	ImGui::Text("Diffuse Intensity");
	ImGui::Text("Specular Intensity");
	ImGui::Text("Attenuation Constant");
	ImGui::Text("Attenuation Linear");
	ImGui::Text("Attenuation Quadratic");

	ImGui::NextColumn();

	ImGui::PushID("DiffuseSlider"); ImGui::SliderFloat("", &m_lightBuffer.m_DiffuseIntensity, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("SpecularSlider"); ImGui::SliderFloat("", &m_lightBuffer.m_specularIntensity, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("AConstantSlider"); ImGui::SliderFloat("", &m_lightBuffer.m_attenuationConstant, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("ALinearSlider"); ImGui::SliderFloat("", &m_lightBuffer.m_attenuationLinear, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("AQuadraticSlider"); ImGui::SliderFloat("", &m_lightBuffer.m_attenuationQuadratic, 0.0f, 10.0f); ImGui::PopID();

	ImGui::NextColumn();

	ImGui::PushID("DiffuseInput"); ImGui::InputFloat("", &m_lightBuffer.m_DiffuseIntensity, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("SpecularInput"); ImGui::InputFloat("", &m_lightBuffer.m_specularIntensity, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("AConstantInput"); ImGui::InputFloat("", &m_lightBuffer.m_attenuationConstant, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("ALinearInput"); ImGui::InputFloat("", &m_lightBuffer.m_attenuationLinear, 0.0f, 10.0f); ImGui::PopID();
	ImGui::PushID("AQuadraticInput"); ImGui::InputFloat("", &m_lightBuffer.m_attenuationQuadratic, 0.0f, 10.0f); ImGui::PopID();

	ImGui::NextColumn();
}

void PointLightComponent::IconUpdate(IconManager& iconManager)
{
	Vector3 screen = RenderCommand::WorldToScreen(GetWorldPosition());
	Icon& lightIcon = iconManager.GetIcon(GetGuid());
	lightIcon.m_position.x = screen.x;
	lightIcon.m_position.y = screen.y;
	lightIcon.m_tint = Vector4(m_lightBuffer.m_Diffuse.x, m_lightBuffer.m_Diffuse.y, m_lightBuffer.m_Diffuse.z, 1.0f);
	lightIcon.m_alignment = Icon::Alignment::Centre;
	lightIcon.RefreshBinds(iconManager); //TODO: Dont do this every frame
}

const LightBuffer_t& PointLightComponent::GetLightBuffer() const
{
	return m_lightBuffer;
}

void PointLightComponent::SetLightBuffer(const LightBuffer_t& buf)
{
	m_lightBuffer = buf;
}

void PointLightComponent::Serialize(YAML::Emitter& Archive)
{
	WorldComponent::Serialize(Archive);

	Archive << YAML::Key << "PointLightComponent";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "Position";
		Archive << YAML::Value << static_cast<Vector3>(m_lightBuffer.m_Position);

		Archive << YAML::Key << "Diffuse";
		Archive << YAML::Value << static_cast<Vector3>(m_lightBuffer.m_Diffuse);

		Archive << YAML::Key << "DiffuseIntensity";
		Archive << YAML::Value << m_lightBuffer.m_DiffuseIntensity;

		Archive << YAML::Key << "SpecularIntensity";
		Archive << YAML::Value << m_lightBuffer.m_specularIntensity;

		Archive << YAML::Key << "AttenuationConstant";
		Archive << YAML::Value << m_lightBuffer.m_attenuationConstant;

		Archive << YAML::Key << "AttenuationLinear";
		Archive << YAML::Value << m_lightBuffer.m_attenuationLinear;

		Archive << YAML::Key << "AttenuationQuadratic";
		Archive << YAML::Value << m_lightBuffer.m_attenuationQuadratic;
	}
	Archive << YAML::EndMap;
}

void PointLightComponent::Deserialize(YAML::Node& Archive)
{
	WorldComponent::Deserialize(Archive);

	if (YAML::Node node = Archive["PointLightComponent"])
	{
		m_lightBuffer.m_Position = node["Position"].as<Vector3>();
		m_lightBuffer.m_Diffuse = node["Diffuse"].as<Vector3>();
		m_lightBuffer.m_DiffuseIntensity = node["DiffuseIntensity"].as<float>();
		m_lightBuffer.m_specularIntensity = node["SpecularIntensity"].as<float>();
		m_lightBuffer.m_attenuationConstant = node["AttenuationConstant"].as<float>();
		m_lightBuffer.m_attenuationLinear = node["AttenuationLinear"].as<float>();
		m_lightBuffer.m_attenuationQuadratic = node["AttenuationQuadratic"].as<float>();
	}
}