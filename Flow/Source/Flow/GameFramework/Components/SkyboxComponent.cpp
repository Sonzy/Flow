#include "Flowpch.h"
#include "SkyboxComponent.h"

#include "Rendering/Core/Camera/Camera.h"
#include "Rendering\Core\Bindables\Topology.h"
#include "Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Rendering\Core\Vertex\VertexLayout.h"
#include "Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Rendering\Core\Bindables\Rasterizer.h"
#include "Rendering\Core\Vertex\VertexBuffer.h"
#include "Rendering/Renderer.h"

#include "Assets/AssetSystem.h"

#include "Assets\Meshes\MeshAsset.h" 

#include <yaml-cpp/yaml.h>

SkyboxComponent::SkyboxComponent()
	: SkyboxComponent("Skybox Component")
{
}

SkyboxComponent::SkyboxComponent(const std::string& Name)
	: RenderableComponent(Name)
{
	m_Mesh = AssetSystem::GetAsset<MeshAsset>(m_MeshPath);
	m_Material = AssetSystem::GetAsset<MaterialAsset>(m_MaterialPath)->GetMaterial();

	RefreshBinds();

	m_SimulatePhysics = false;
}


DirectX::XMMATRIX SkyboxComponent::GetTransformXM() const
{
	const float farPlane = RenderCommand::GetFarPlaneRef();

	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition();
	return DirectX::XMMatrixScaling(farPlane, farPlane, farPlane) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}

void SkyboxComponent::Render()
{
	Renderer::Submit(this);

	WorldComponent::Render();
}

void SkyboxComponent::RefreshBinds()
{
	m_Techniques.clear();

	Technique Standard("SkyboxComponent_Standard");
	{
		Step MainStep(RenderPass::FrontFaceCulling);

		VertexLayout MeshLayout;
		m_Mesh->GetMesh(0)->GenerateBinds(MeshLayout);

		m_IndexBuffer = m_Mesh->GetMesh(0)->m_IndexBuffer;
		m_VertexBuffer = m_Mesh->GetMesh(0)->m_BindableVBuffer;
		m_Topology = m_Mesh->GetMesh(0)->m_Topology;

		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(new TransformConstantBuffer(this)); //TODO: need to clean this up
		Standard.AddStep(MainStep);
	}
	AddTechnique(Standard);
}

const std::string& SkyboxComponent::GetMaterialPath()
{
	return m_MaterialPath;
}

void SkyboxComponent::SetMaterial(const std::string& path)
{
	m_Material->SetTexture(path);
	RefreshBinds();
}

void SkyboxComponent::DrawComponentDetailsWindow()
{
	WorldComponent::DrawComponentDetailsWindow();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Skybox Component");


	std::vector<const char*> Assets = AssetSystem::BuildAssetList<TextureAsset>();
	static int CurrentItem = 0;
	const char* currentItem = m_MaterialPath.c_str();

	if (ImGui::BeginCombo("Material Selector - Skybox", currentItem, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < Assets.size(); n++)
		{
			bool is_selected = (currentItem == Assets[n]);

			if (ImGui::Selectable(Assets[n], is_selected))
			{
				currentItem = Assets[n];

				m_MaterialPath = currentItem;
				m_Material->SetTexture(currentItem);
				RefreshBinds();
			}

			// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void SkyboxComponent::Serialize(YAML::Emitter& Archive)
{
	WorldComponent::Serialize(Archive);

	Archive << YAML::Key << "SkyboxComponent";
	Archive << YAML::BeginMap;
	{
		Archive << YAML::Key << "SkyboxTexture";
		Archive << YAML::Value << m_MaterialPath;
	}
	Archive << YAML::EndMap;
}

void SkyboxComponent::Deserialize(YAML::Node& Archive)
{
	WorldComponent::Deserialize(Archive);

	if (YAML::Node node = Archive["SkyboxComponent"])
	{
		SetMaterial(node["SkyboxTexture"].as<std::string>());
	}
}