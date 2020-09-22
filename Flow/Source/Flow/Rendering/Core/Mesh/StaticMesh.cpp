#include "Flowpch.h"
#include "StaticMesh.h"
#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Flow\Assets\Shaders\ShaderAsset.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\Rendering\Core\Materials\Material.h"
StaticMesh::StaticMesh(const std::string& LocalPath)
{

}

void StaticMesh::InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride)
{
	MeshAsset* m_MeshAsset = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
	CHECK_RETURN(!m_MeshAsset, "StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath);
	Mesh* m_Mesh = m_MeshAsset->GetMesh(0);
	VertexLayout MeshLayout;

	Technique Standard("StaticMesh_Standard");
	{
		Step MainStep(0);

		//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
		m_Mesh->GenerateBinds(MeshLayout);
		_VertexBuffer = m_Mesh->m_BindableVBuffer;
		_IndexBuffer = m_Mesh->m_IndexBuffer;
		_Topology = m_Mesh->m_Topology;

		MaterialOverride->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX StaticMesh::GetTransformXM() const
{
	return DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(_Rotation.Pitch, _Rotation.Yaw, _Rotation.Roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(_Position.x, _Position.y, _Position.z);// * //Move relative to origin
}