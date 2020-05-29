#include "Flowpch.h"
#include "PointLight.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Mesh\StaticMesh.h"
#include "Flow\Assets\AssetSystem.h"

PointLight::PointLight(float LightRadius)
	: _Mesh("Box"), _PixelConstantBuffer(0)
{
	Reset();

	Material* Material = AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour")->GetMaterial();

	_Mesh.InitialiseStaticMesh("Sphere", Material);
	_Mesh.SetScale(10.0f);
}

void PointLight::Reset()
{
	_ConstantBuffer = {
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
	auto Copy = _ConstantBuffer;
	const auto Position = DirectX::XMLoadFloat3(&_ConstantBuffer.pos);

	DirectX::XMStoreFloat3(&Copy.pos, DirectX::XMVector3Transform(Position, ViewMatrix));
	_Mesh.SetPosition(Vector(_ConstantBuffer.pos.x, _ConstantBuffer.pos.y, _ConstantBuffer.pos.z));

	_PixelConstantBuffer.Update(Copy);
	_PixelConstantBuffer.Bind();

	if (_DrawMesh)
	{
		Renderer::Submit(&_Mesh);
	}
}

void PointLight::RenderControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &_ConstantBuffer.pos.x, -600.0f, 600.0f, "%.1f");
		ImGui::SliderFloat("Y", &_ConstantBuffer.pos.y, -600.0f, 600.0f, "%.1f");
		ImGui::SliderFloat("Z", &_ConstantBuffer.pos.z, -600.0f, 600.0f, "%.1f");

		ImGui::Text("Intensity/Colour");
		ImGui::SliderFloat("Intensity", &_ConstantBuffer.diffuseIntensity, 0.1f, 5.0f, "%.2f");
		ImGui::ColorEdit3("Diffuse", &_ConstantBuffer.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &_ConstantBuffer.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &_ConstantBuffer.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &_ConstantBuffer.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &_ConstantBuffer.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}