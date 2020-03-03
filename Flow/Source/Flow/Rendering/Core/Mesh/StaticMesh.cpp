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
#include "Flow\Assets\Materials\Temporary\Mat_Hat_FancyMan.h"

namespace Flow
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> StaticMesh::m_VertexLayout;

	StaticMesh::StaticMesh(const std::string& LocalPath)
	{

	}

	void StaticMesh::InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride)
	{
		if (!StaticInitialised())
		{
			//Bind Topology
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
			SetIndexFromStatic();


		// Define Vertex Layout
		VertexLayout Layout;
		Layout.Append(ElementType::Position3D);
		Layout.Append(ElementType::Normal);
		Layout.Append(ElementType::Texture2D);

		// Define Vertex Buffer information
		VertexBuffer VBuffer(Layout);

		//Store D3D layout
		m_VertexLayout = Layout.GetD3DLayout();

		MeshAsset* m_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
		CHECK_RETURN(!m_Mesh, "StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath);

		for (auto& Vertex : m_Mesh->GetVertices())
		{
			VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
				DirectX::XMFLOAT3{ Vertex.Position.X ,  Vertex.Position.Y,  Vertex.Position.Z },
				DirectX::XMFLOAT3{ Vertex.Normal.X ,  Vertex.Normal.Y,  Vertex.Normal.Z },
				DirectX::XMFLOAT2{ Vertex.TexCoord.X,  Vertex.TexCoord.Y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(m_Mesh->GetNumFaces() * 3); //Using triangles, change for quads
		for (auto& Face : m_Mesh->GetFaces())
		{
			assert(Face.m_NumIndices == 3);
			indices.push_back(Face.m_Indices[0]);
			indices.push_back(Face.m_Indices[1]);
			indices.push_back(Face.m_Indices[2]);
		}

		//Add Vertex Buffer Bind
		AddStaticBindable(std::make_unique<BindableVertexBuffer>(VBuffer));

		if (!MaterialOverride)
		{
			m_Material = new Mat_Hat_FancyMan();
			m_Material->BindMaterial(this, m_VertexLayout);
		}
		else
			MaterialOverride->BindMaterial(this, m_VertexLayout);

		//Bind Index Buffer
		if(GetIndexBuffer()) //TODO: Whole framework needs changing, canny have 1 index buffer per. Might have to mvoe all this to static mesh asset
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

		//Bind Transform
		AddBind(std::make_unique<TransformConstantBuffer>(this));
	}

	DirectX::XMMATRIX StaticMesh::GetTransformXM() const
	{
		return DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(m_Position.X, m_Position.Y, m_Position.Z) * //Move relative to origin
			DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f); //Rotate around world centre
	}
}