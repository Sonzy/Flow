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
		_VertexBuffer = m_Mesh->_BindableVBuffer;
		_IndexBuffer = m_Mesh->_IndexBuffer;
		_Topology = m_Mesh->_Topology;

		MaterialOverride->BindMaterial(&MainStep, MeshLayout);

		MainStep.AddBindable(std::make_shared<TransformConstantBuffer>(this));

		Standard.AddStep(std::move(MainStep));
	}
	AddTechnique(Standard);
}

DirectX::XMMATRIX StaticMesh::GetTransformXM() const
{
	return DirectX::XMMatrixScaling(_Scale.X, _Scale.Y, _Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(_Rotation.Pitch, _Rotation.Yaw, _Rotation.Roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(_Position.X, _Position.Y, _Position.Z);// * //Move relative to origin
}