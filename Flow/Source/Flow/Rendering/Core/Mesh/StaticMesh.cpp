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

namespace Flow
{
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
				DirectX::XMFLOAT3{ Vertex.Position_.X ,  Vertex.Position_.Y,  Vertex.Position_.Z },
				DirectX::XMFLOAT3{ Vertex.Normal_.X ,  Vertex.Normal_.Y,  Vertex.Normal_.Z },
				DirectX::XMFLOAT2{ Vertex.TexCoord_.X,  Vertex.TexCoord_.Y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(m_Mesh->GetNumFaces() * 3); //Using triangles, change for quads
		for (auto& Face : m_Mesh->GetFaces())
		{
			assert(Face.NumIndices_ == 3);
			indices.push_back(Face.Indices_[0]);
			indices.push_back(Face.Indices_[1]);
			indices.push_back(Face.Indices_[2]);
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
		return DirectX::XMMatrixScaling(Scale_.X, Scale_.Y, Scale_.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(Rotation_.Pitch, Rotation_.Yaw, Rotation_.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(Position_.X, Position_.Y, Position_.Z);// * //Move relative to origin
	}
}