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
	AddBind(Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Define Vertex Layout
	VertexLayout Layout;
	Layout.Append(ElementType::Position3D);
	Layout.Append(ElementType::Normal);
	Layout.Append(ElementType::Texture2D);

	// Define Vertex Buffer information
	VertexBuffer VBuffer(Layout);

	MeshAsset* m_MeshAsset = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
	CHECK_RETURN(!m_MeshAsset, "StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath);
	Mesh* m_Mesh = m_MeshAsset->GetMesh(0);

	for (auto& Vertex : m_Mesh->GetVertices())
	{
		VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
			DirectX::XMFLOAT3{ Vertex._Position.X ,  Vertex._Position.Y,  Vertex._Position.Z },
			DirectX::XMFLOAT3{ Vertex._Normal.X ,  Vertex._Normal.Y,  Vertex._Normal.Z },
			DirectX::XMFLOAT2{ Vertex._TexCoord.X,  Vertex._TexCoord.Y }
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(m_Mesh->GetNumFaces() * 3); //Using triangles, change for quads
	for (auto& Face : m_Mesh->GetFaces())
	{
		assert(Face._NumIndices == 3);
		indices.push_back(Face._Indices[0]);
		indices.push_back(Face._Indices[1]);
		indices.push_back(Face._Indices[2]);
	}

	//Add Vertex Buffer Bind
	AddBind(BindableVertexBuffer::Resolve(LocalPath, VBuffer));

	MaterialOverride->BindMaterial(this, Layout);

	//Bind Index Buffer
	AddBind(IndexBuffer::Resolve(LocalPath, indices));

	//Bind Transform
	AddBind(std::make_shared<TransformConstantBuffer>(this));
}

DirectX::XMMATRIX StaticMesh::GetTransformXM() const
{
	return DirectX::XMMatrixScaling(_Scale.X, _Scale.Y, _Scale.Z) *
		DirectX::XMMatrixRotationRollPitchYaw(_Rotation.Pitch, _Rotation.Yaw, _Rotation.Roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(_Position.X, _Position.Y, _Position.Z);// * //Move relative to origin
}