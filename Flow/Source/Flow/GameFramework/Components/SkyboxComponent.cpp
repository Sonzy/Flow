#include "pch.h"
#include "SkyboxComponent.h"

#include "Rendering/Core/Camera/Camera.h"
#include "Rendering/Core/Bindables/Topology.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Bindables/ConstantBuffers/TransformConstantBuffer.h"
#include "Rendering/Core/Bindables/Rasterizer.h"
#include "Rendering/Core/Vertex/VertexBufferData.h"
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
	const float farPlane = Renderer::GetFarPlaneRef();

	DirectX::XMFLOAT3 CamPos = Renderer::GetMainCamera()->GetCameraPosition();
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
	m_techniques.clear();

	Technique Standard("SkyboxComponent_Standard");
	{
		Step MainStep(RenderPass::FrontFaceCulling);

		VertexLayout MeshLayout;
		m_Mesh->GetMesh(0)->GenerateBinds(MeshLayout);

		m_indexBuffer = m_Mesh->GetMesh(0)->m_IndexBuffer;
		m_vertexBuffer = m_Mesh->GetMesh(0)->m_BindableVBuffer;
		m_topology = m_Mesh->GetMesh(0)->m_Topology;

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


	std::vector<const char*> textures = AssetSystem::BuildAssetList<TextureAsset>();
	const char* currentTextureName = m_textureName.c_str();

	if (ImGui::BeginCombo("Textures", currentTextureName, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < textures.size(); n++)
		{
			bool is_selected = (currentTextureName == textures[n]);

			if (ImGui::Selectable(textures[n], is_selected))
			{
				currentTextureName = textures[n];

				m_textureName = currentTextureName;
				m_Material->SetTexture(currentTextureName);
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

	std::vector<const char*> materials = AssetSystem::BuildAssetList<MaterialAsset>();
	const char* currentTexutre = m_MaterialPath.c_str();
	if (ImGui::BeginCombo("Materials", currentTexutre, 0)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < materials.size(); n++)
		{
			bool is_selected = (currentTexutre == materials[n]);

			if (ImGui::Selectable(materials[n], is_selected))
			{
				currentTexutre = materials[n];

				m_MaterialPath = currentTexutre;
				MaterialAsset* FoundMat = AssetSystem::GetAsset<MaterialAsset>(m_MaterialPath);
				if (!FoundMat)
				{
					FLOW_ENGINE_ERROR("StaticMeshComponent::SetMaterial: Failed to find material with name %s", m_MaterialPath.c_str());
					return;
				}
				m_Material = FoundMat->GetMaterial();

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