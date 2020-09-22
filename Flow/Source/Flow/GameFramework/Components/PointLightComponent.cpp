#include "Flowpch.h"
#include "PointLightComponent.h"
#include "ThirdParty/ImGui/imgui.h"

PointLightComponent::PointLightComponent()
	: RenderableComponent("Unnamed Point Light Component"), _PixelCB(0)
{
}

PointLightComponent::PointLightComponent(const std::string& ComponentName)
	: RenderableComponent(ComponentName), _PixelCB(0)
{
	_CB = {
	{ 1.5f,100.0f,-4.5f },
	{ 0.05f,0.05f,0.05f },
	{ 1.0f,1.0f,1.0f },
	1.0f,
	1.0f,
	0.045f,
	0.0075f,
	};
}

void PointLightComponent::Render()
{
	//Update from component position
	_CB._Position = GetWorldPosition();

	//Create a copy and transform the copied position with the view matrix
	auto Copy = _CB;
	const auto Position = DirectX::XMLoadFloat3(&_CB._Position);

	//TODO: Might have to recalculate
	DirectX::XMStoreFloat3(&Copy._Position, DirectX::XMVector3Transform(Position, RenderCommand::GetMainCamera()->GetCachedView()));

	//Update the transformed position to the shader
	_PixelCB.Update(Copy);
	_PixelCB.Bind();
}

void PointLightComponent::DrawComponentDetailsWindow()
{
	ImGui::TextColored(IMGUI_YELLOW, "Point Light Settings");
	ImGui::ColorPicker3("Light Diffuse Colour", reinterpret_cast<float*>(&_CB._Diffuse));		
	ImGui::ColorPicker3("Ambient Colour", reinterpret_cast<float*>(&_CB._Ambient));
	ImGui::InputFloat("Diffuse Intensity", reinterpret_cast<float*>(&_CB._DiffuseIntensity));				
	ImGui::InputFloat("Attenuation Constant", reinterpret_cast<float*>(&_CB._AttenuationConstant));				
	ImGui::InputFloat("Attenuation Linear", reinterpret_cast<float*>(&_CB._AttenuationLinear));				
	ImGui::InputFloat("Attenuation Quadratic", reinterpret_cast<float*>(&_CB._AttenuationQuadratic));				
}
