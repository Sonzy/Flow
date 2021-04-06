//= Includes ==================================

#include "pch.h"
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

#include "Maths/Maths.h"

//= Class Definition - Static Mesh =====================================

StaticMesh::StaticMesh(const std::string& LocalPath)
{

}

void StaticMesh::InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride)
{
	MeshAsset* m_MeshAsset = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
	CHECK_RETURN(!m_MeshAsset, ("StaticMesh::StaticMesh: Failed to get asset (%s)", LocalPath.c_str()));
	Mesh* m_Mesh = m_MeshAsset->GetMesh(0);
	VertexLayout MeshLayout;

	Technique Standard("StaticMesh_Standard");
	{
		Step MainStep(RenderPass::Main);

		//Set the bindables for this specific object (Topology, Indices, VertexBuffer) 
		m_Mesh->GenerateBinds(MeshLayout);
		m_VertexBuffer = m_Mesh->m_BindableVBuffer;
		m_IndexBuffer = m_Mesh->m_IndexBuffer;
		m_Topology = m_Mesh->m_Topology;

		MaterialOverride->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(new TransformConstantBuffer(this));

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX StaticMesh::GetTransformXM() const
{
	Rotator rads = Rotator::AsRadians(m_Rotation);
	return 	DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) *
		DirectX::XMMatrixRotationQuaternion(Maths::EulersToQuaternion(rads)) *
		//TODO: XMMAtrixRotationRollPitchYaw doesn't work here. Not sure what is different between the conversion formulae
		DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}