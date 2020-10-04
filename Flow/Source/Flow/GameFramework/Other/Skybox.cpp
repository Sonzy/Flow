#include "Flowpch.h"
#include "Skybox.h"

#include "Flow\Utils\HelperMacros.h"
#include "Flow\Logging\Log.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Rendering\Core\Bindables\Topology.h"
#include "Flow\Rendering\Core\Bindables\BindableVertexBuffer.h"
#include "Flow\Rendering\Core\Materials\Material.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\TransformConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\Rasterizer.h"

#include "Flow\Rendering\Core\Vertex\VertexLayout.h"
#include "Flow\Rendering\Core\Vertex\VertexBuffer.h"

#include "Flow\Rendering\Core\Camera\Camera.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

Skybox::Skybox()
{
	m_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(m_MeshPath));
	m_Material = AssetSystem::GetAsset<MaterialAsset>(m_MaterialPath)->GetMaterial();

	CHECK_RETURN(!m_Mesh, "Skybox::Skybox: Failed to load mesh");
	CHECK_RETURN(!m_Material, "Skybox::Skybox: Failed to load material");

	Technique Standard("Skybox_Standard");
	{
		Step MainStep(1);

		VertexLayout MeshLayout;
		MainStep.AddBindables(m_Mesh->GetMesh(0)->GenerateBinds(MeshLayout));
		m_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX Skybox::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition();
	return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}