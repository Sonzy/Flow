#include "Flowpch.h"
#include "Skybox.h"

#include "Flow\Helper\HelperMacros.h"
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
	_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(_MeshPath));
	_Material = AssetSystem::GetAsset<MaterialAsset>(_MaterialPath)->GetMaterial();

	CHECK_RETURN(!_Mesh, "Skybox::Skybox: Failed to load mesh");
	CHECK_RETURN(!_Material, "Skybox::Skybox: Failed to load material");

	Technique Standard;
	{
		Step MainStep(1);

		VertexLayout MeshLayout;
		MainStep.AddBindables(_Mesh->GetMesh(0)->GenerateBinds(MeshLayout));
		_Material->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));
	}
	AddTechnique(std::move(Standard));
}

DirectX::XMMATRIX Skybox::GetTransformXM() const
{
	DirectX::XMFLOAT3 CamPos = RenderCommand::GetMainCamera()->GetCameraPosition().ToDXFloat3();
	return DirectX::XMMatrixScaling(300.0f, 300.0f, 300.0f) *
		DirectX::XMMatrixTranslation(CamPos.x, CamPos.y, CamPos.z);
}