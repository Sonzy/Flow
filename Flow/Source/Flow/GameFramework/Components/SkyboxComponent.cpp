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
	_Mesh = AssetSystem::GetAsset<MeshAsset>(_MeshPath);
	_Material = AssetSystem::GetAsset<MaterialAsset>(_MaterialPath)->GetMaterial();

	Technique Standard("SkyboxComponent_Standard");
	{
		Step MainStep(1);

		VertexLayout MeshLayout;
		_Mesh->GetMesh(0)->GenerateBinds(MeshLayout);

		_IndexBuffer = _Mesh->GetMesh(0)->_IndexBuffer;
		_VertexBuffer = _Mesh->GetMesh(0)->_BindableVBuffer;
		_Topology = _Mesh->GetMesh(0)->_Topology;

		_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));
		Standard.AddStep(MainStep);
	}
	AddTechnique(Standard);

	_SimulatePhysics = false;
}


DirectX::XMMATRIX SkyboxComponent::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition().ToDXFloat3();
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
