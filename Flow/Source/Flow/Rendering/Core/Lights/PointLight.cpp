#include "Flowpch.h"
#include "PointLight.h"
#include "ThirdParty\ImGui\imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\Core\Mesh\StaticMesh.h"
#include "Flow\Assets\AssetSystem.h"

namespace Flow
{
	PointLight::PointLight(float LightRadius)
		: Mesh_("Box"), PixelConstantBuffer_(0)
	{
		Reset();

		Flow::Material* Material = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_FlatColour")->GetMaterial();

		Mesh_.InitialiseStaticMesh("Sphere", Material);
		Mesh_.SetScale(10.0f);

		DrawMesh_ = false;
	}

	void PointLight::Reset()
	{
		ConstantBuffer_ = {
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
		auto Copy = ConstantBuffer_;
		const auto Position = DirectX::XMLoadFloat3(&ConstantBuffer_.pos);

		DirectX::XMStoreFloat3(&Copy.pos, DirectX::XMVector3Transform(Position, ViewMatrix));
		Mesh_.SetPosition(Vector(ConstantBuffer_.pos.x, ConstantBuffer_.pos.y, ConstantBuffer_.pos.z));

		PixelConstantBuffer_.Update(Copy);
		PixelConstantBuffer_.Bind();

		if (DrawMesh_)
		{
			Renderer::Submit(&Mesh_);
		}
	}

	void PointLight::RenderControlWindow()
	{
		if (ImGui::Begin("Light"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &ConstantBuffer_.pos.x, -600.0f, 600.0f, "%.1f");
			ImGui::SliderFloat("Y", &ConstantBuffer_.pos.y, -600.0f, 600.0f, "%.1f");
			ImGui::SliderFloat("Z", &ConstantBuffer_.pos.z, -600.0f, 600.0f, "%.1f");

			ImGui::Text("Intensity/Colour");
			ImGui::SliderFloat("Intensity", &ConstantBuffer_.diffuseIntensity, 0.1f, 5.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse", &ConstantBuffer_.diffuseColor.x);
			ImGui::ColorEdit3("Ambient", &ConstantBuffer_.ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &ConstantBuffer_.attConst, 0.05f, 10.0f, "%.2f", 4);
			ImGui::SliderFloat("Linear", &ConstantBuffer_.attLin, 0.0001f, 4.0f, "%.4f", 8);
			ImGui::SliderFloat("Quadratic", &ConstantBuffer_.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}
}

