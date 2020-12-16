//= Includes ====================================================

#include "Flowpch.h"
#include "PointLight.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Rendering\Renderer.h"
#include "Rendering\Core\Mesh\StaticMesh.h"
#include "Assets\AssetSystem.h"

//= Class (PointLight) Definition ================================

PointLight::PointLight(float LightRadius)
	:  m_PixelConstantBuffer(1)
{
	Reset();
}

void PointLight::Reset()
{
	m_ConstantBuffer = {
		{ 1.5f,100.0f,-4.5f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::BindLight(DirectX::FXMMATRIX ViewMatrix)
{
	auto Copy = m_ConstantBuffer;
	const auto Position = DirectX::XMLoadFloat3(&m_ConstantBuffer.pos);

	DirectX::XMStoreFloat3(&Copy.pos, DirectX::XMVector3Transform(Position, ViewMatrix));


	m_PixelConstantBuffer.Update(Copy);
	m_PixelConstantBuffer.Bind();
}

void PointLight::RenderControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_ConstantBuffer.pos.x, -600.0f, 600.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_ConstantBuffer.pos.y, -600.0f, 600.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_ConstantBuffer.pos.z, -600.0f, 600.0f, "%.1f");

		ImGui::Text("Intensity/Colour");
		ImGui::SliderFloat("Intensity", &m_ConstantBuffer.diffuseIntensity, 0.1f, 5.0f, "%.2f");
		ImGui::ColorEdit3("Diffuse", &m_ConstantBuffer.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &m_ConstantBuffer.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &m_ConstantBuffer.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &m_ConstantBuffer.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &m_ConstantBuffer.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}