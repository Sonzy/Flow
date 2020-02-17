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
		if (!StaticInitialised())
		{
			// Define Vertex Layout
			VertexLayout Layout;
			Layout.Append(ElementType::Position3D);
			Layout.Append(ElementType::Normal);
			Layout.Append(ElementType::Texture2D);

			// Define Vertex Buffer information
			VertexBuffer VBuffer(Layout);

			MeshAsset* m_Mesh = reinterpret_cast<MeshAsset*>(AssetSystem::GetAsset(LocalPath));
			CHECK_RETURN(!m_Mesh, "StaticMesh::StaticMesh: Failed to get asset ({0})", LocalPath);

			for (auto& Vertex : m_Mesh->GetVertices())
			{
				VBuffer.EmplaceBack( //TODO: Dont actually need to know whats in here, just need to know the stride and offsets
					DirectX::XMFLOAT3{ Vertex.Position.X ,  Vertex.Position.Y,  Vertex.Position.Z },
					DirectX::XMFLOAT3{ Vertex.Normal.X ,  Vertex.Normal.Y,  Vertex.Normal.Z },
					DirectX::XMFLOAT2 { Vertex.TexCoord.X,  Vertex.TexCoord.Y}
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

			//Bind Material
			m_Material = new Material(this);
			m_Material->SetTexture("CharacterTexture");
			m_Material->SetPixelShader("TexturedPS");
			m_Material->SetVertexShader("TexturedVS");
			m_Material->BindMaterial(VBuffer);

			//Bind Index Buffer
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

			//Bind Topology
			AddStaticBindable (std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
			SetIndexFromStatic();
			
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