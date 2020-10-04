//= Includes ==================================

#include "Flowpch.h"
#include "StaticMesh.h"
#include "Rendering/Core/Vertex/VertexLayout.h"
#include "Rendering/Core/Vertex/VertexBuffer.h"

#include "Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Assets\Shaders\ShaderAsset.h"

#include "Assets\AssetSystem.h"

#include "Rendering\Core\Materials\Material.h"
#include "Assets/Meshes/MeshAsset.h"

//= Class Definition - Static Mesh =====================================

StaticMesh::StaticMesh(const std::string& LocalPath)
{

}

void StaticMesh::InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride)
{
	MeshAsset* m_MeshAsset = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
	CHECK_RETURN(!m_MeshAsset, ("StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath));
	Mesh* m_Mesh = m_MeshAsset->GetMesh(0);
	VertexLayout MeshLayout;

	Technique Standard("StaticMesh_Standard");
	{
		Step MainStep(0);

		//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
		m_Mesh->GenerateBinds(MeshLayout);
		m_VertexBuffer = m_Mesh->m_BindableVBuffer;
		m_IndexBuffer = m_Mesh->m_IndexBuffer;
		m_Topology = m_Mesh->m_Topology;

		MaterialOverride->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX StaticMesh::GetTransformXM() const
{
	return DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);// * //Move relative to origin
}