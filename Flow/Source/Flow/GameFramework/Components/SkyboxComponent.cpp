#include "Flowpch.h"
#include "SkyboxComponent.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"
#include "Flow\Rendering\Core\Vertex\VertexBuffer.h"
#include "Flow/Rendering/Renderer.h"

#include "Flow/Assets/AssetSystem.h"

#include "Flow\Assets\Meshes\MeshAsset.h" 

SkyboxComponent::SkyboxComponent()
	: SkyboxComponent("Skybox Component")
{
}

SkyboxComponent::SkyboxComponent(const std::string& Name)
	: RenderableComponent(Name)
{
	m_Mesh = AssetSystem::GetAsset<MeshAsset>(m_MeshPath);
	m_Material = AssetSystem::GetAsset<MaterialAsset>(m_MaterialPath)->GetMaterial();

	Technique Standard("SkyboxComponent_Standard");
	{
		Step MainStep(1);

		VertexLayout MeshLayout;
		m_Mesh->GetMesh(0)->GenerateBinds(MeshLayout);

		m_IndexBuffer = m_Mesh->GetMesh(0)->m_IndexBuffer;
		m_VertexBuffer = m_Mesh->GetMesh(0)->m_BindableVBuffer;
		m_Topology = m_Mesh->GetMesh(0)->m_Topology;

		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));
		Standard.AddStep(MainStep);
	}
	AddTechnique(Standard);

	m_SimulatePhysics = false;
}


DirectX::XMMATRIX SkyboxComponent::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition();
	return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}

void SkyboxComponent::Render()
{
	Renderer::Submit(this);

	WorldComponent::Render();
}

std::string SkyboxComponent::GetClassSerializationUID(std::ofstream* Archive)
{
	return typeid(SkyboxComponent).name();
}
