#include "pch.h"
#include "Skybox.h"

#include "Framework\Utils\HelperMacros.h"
#include "Framework\Logging\Log.h"

#include "Assets\AssetSystem.h"
#include "Assets\Meshes\MeshAsset.h"

#include "Rendering\Core\Bindables\Topology.h"
#include "Rendering\Core\Bindables\VertexBuffer.h"
#include "Rendering\Core\Materials\Material.h"
#include "Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Rendering\Core\Bindables\Rasterizer.h"

#include "Rendering\Core\Vertex\VertexLayout.h"
#include "Rendering\Core\Vertex\VertexBufferData.h"

#include "Rendering\Core\Camera\Camera.h"
#include "GameFramework\Components\CameraComponent.h"

Skybox::Skybox()
{
	m_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(m_MeshPath));
	m_Material = AssetSystem::GetAsset<MaterialAsset>(m_MaterialPath)->GetMaterial();

	CHECK_RETURN(!m_Mesh, "Skybox::Skybox: Failed to load mesh");
	CHECK_RETURN(!m_Material, "Skybox::Skybox: Failed to load material");

	Technique Standard("Skybox_Standard");
	{
		Step MainStep(RenderPass::Main);

		MainStep.AddBindable(Rasterizer::Resolve(Rasterizer::Cull_Front));

		VertexLayout MeshLayout;
		MainStep.AddBindables(m_Mesh->GetMesh(0)->GenerateBinds(MeshLayout));
		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(new TransformConstantBuffer(this));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX Skybox::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = Renderer::GetMainCamera()->GetCameraPosition();
	return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}