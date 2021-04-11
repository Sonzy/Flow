#include "pch.h"
#include "DirectionalLightComponent.h"
#include "Maths/Vector3.h"
#include "ThirdParty/ImGui/imgui.h"

#include "Assets/Materials/MaterialCommon.h"

#include "Rendering/Renderer.h"
#include "Rendering/Core/Camera/Camera.h"

#if WITH_EDITOR
#include "Assets/AssetSystem.h"
#include "Editor/Editor.h"
#include "Editor/IconManager.h"
#include "Rendering/Core/Mesh/StaticMesh.h"
#include "Rendering/Renderer.h"
#endif


#include <yaml-cpp/yaml.h>
#include "Framework/Utils/YamlSerializer.h"

DirectionalLightComponent::DirectionalLightComponent()
	: DirectionalLightComponent("DirectionalLightComponent")
{
}

DirectionalLightComponent::DirectionalLightComponent(const std::string& ComponentName)
	: RenderableComponent(ComponentName)
	, m_lightPixelBuffer(MaterialCommon::Register::DirectionalLightProperties)
{
#if WITH_EDITOR
	m_arrowPointer = new StaticMesh("");
	m_arrowPointer->InitialiseStaticMesh("ArrowZForward", AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColor_Green")->GetMaterial());
#endif
}

void DirectionalLightComponent::OnRegistered()
{
#if WITH_EDITOR
	IconManager::IconData iconData;
	iconData.m_texture = AssetSystem::GetAsset<TextureAsset>("Icon_DirectionalLight"); //TODO: Make new icon
	Editor::Get().GetUIComponent<IconManager>()->RegisterIcon(GetGuid(), iconData);
#endif
}

void DirectionalLightComponent::Render()
{
#if WITH_EDITOR
	//Update from component position
	m_lightBuffer.m_direction = m_arrowPointer->m_Rotation.GetForwardVector();
	m_arrowPointer->m_Position = m_RelativeTransform.m_Position + Vector3(m_lightBuffer.m_direction).Normalize() * 1.0f;
#else
	m_lightBuffer.m_direction = GetWorldRotation().GetForwardVector();
#endif //WITH_EDITOR

	//Create a copy and transform the copied position with the view matrix
	DirectionalLightBuffer_t Copy = m_lightBuffer;

	//Rotate light in view space
	CameraBase* cam = Renderer::GetMainCamera();
	Transform temp = cam->GetCameraTransform();
	cam->MoveCamera(Transform(0.0f, temp.m_Rotation, temp.m_Scale));

	const DirectX::XMVECTOR dir = DirectX::XMLoadFloat3(&m_lightBuffer.m_direction);
	DirectX::XMStoreFloat3(&Copy.m_direction, DirectX::XMVector3Transform(dir, Renderer::GetMainCamera()->GetViewMatrix())); // modelView
	
	cam->MoveCamera(temp);

	//Update the transformed position to the shader
	m_lightPixelBuffer.Update(Copy);
	m_lightPixelBuffer.Bind();

#if WITH_EDITOR
	m_arrowPointer->m_Rotation = GetWorldRotation();
	m_arrowPointer->m_Scale = GetWorldScale();
	Renderer::Submit(m_arrowPointer);
#endif // WITH_EDITOR
}

void DirectionalLightComponent::DrawComponentDetailsWindow()
{
	ImGui::InputFloat3("Direction", (float*)&m_lightBuffer.m_direction);
	ImGui::ColorEdit3("Light Color", (float*)&m_lightBuffer.m_diffuseColor);
	ImGui::ColorEdit3("Ambient Color", (float*)&m_lightBuffer.m_ambientColor);
	ImGui::ColorEdit3("Specular Color", (float*)&m_lightBuffer.m_specularColor);

	ImGui::InputFloat("Ambient Intensity", &m_lightBuffer.m_ambientIntensity);
	ImGui::InputFloat("Specular Intensity", &m_lightBuffer.m_specularIntensity);
}

#if WITH_EDITOR
void DirectionalLightComponent::IconUpdate(IconManager& iconManager)
{
	Vector3 screen = Renderer::WorldToScreen(GetWorldPosition());
	Icon& lightIcon = iconManager.GetIcon(GetGuid());
	lightIcon.m_position.x = screen.x;
	lightIcon.m_position.y = screen.y;
	lightIcon.m_tint = Vector4(m_lightBuffer.m_diffuseColor.x, m_lightBuffer.m_diffuseColor.y, m_lightBuffer.m_diffuseColor.z, 1.0f);
	lightIcon.m_alignment = Icon::Alignment::Centre;
	lightIcon.RefreshBinds(iconManager); //TODO: Dont do this every frame
}
#endif // WITH_EDITOR

const DirectionalLightBuffer_t& DirectionalLightComponent::GetLightBuffer() const
{
	return m_lightBuffer;
}

void DirectionalLightComponent::SetLightBuffer(const DirectionalLightBuffer_t& buf)
{
	m_lightBuffer = buf;
}

void DirectionalLightComponent::Serialize(YAML::Emitter& Archive)
{
	WorldComponent::Serialize(Archive);
}

void DirectionalLightComponent::Deserialize(YAML::Node& Archive)
{
	WorldComponent::Deserialize(Archive);
}
