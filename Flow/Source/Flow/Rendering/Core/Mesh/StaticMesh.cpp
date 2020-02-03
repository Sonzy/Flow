#include "Flowpch.h"
#include "StaticMesh.h"
#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Rendering\Core\ModelLoader.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>


namespace Flow
{
	StaticMesh::StaticMesh(const std::string& LocalPath)
	{
		if (!StaticInitialised())
		{
			// Define Vertex Layout
			VertexLayout Layout;
			Layout.Append(ElementType::Position3D);

			// Define Vertex Buffer information
			VertexBuffer VBuffer(Layout);


			Assimp::Importer Importer;
			const aiScene* Model = Importer.ReadFile(LocalPath,
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices);

			//Load the data from each vertex
			const aiMesh* Mesh = Model->mMeshes[0];
			for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
			{
				VBuffer.EmplaceBack(
					DirectX::XMFLOAT3{ Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z }
				);
			}

			//Load the indices
			std::vector<unsigned short> indices;
			indices.reserve(Mesh->mNumFaces * 3); //Using triangles, change for quads
			for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
			{
				const auto& face = Mesh->mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			//Add Vertex Buffer Bind
			AddStaticBindable(std::make_unique<BindableVertexBuffer>(VBuffer));

			//Bind Shaders
			auto vShader = std::make_unique<VertexShader>(L"C:/Users/Sonny/Documents/Personal Projects/Flow Engine/Flow/Source/Flow/Rendering/Core/Shaders/SolidColorVS.cso");
			auto vShaderByteCode = vShader->GetByteCode();
			AddStaticBindable(std::move(vShader));
			AddStaticBindable(std::make_unique<PixelShader>(L"C:/Users/Sonny/Documents/Personal Projects/Flow Engine/Flow/Source/Flow/Rendering/Core/Shaders/SolidColourPS.cso"));

			//Bind Index Buffer
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

			//Bind Input Layout
			AddStaticBindable(std::make_unique<InputLayout>(VBuffer.GetLayout().GetD3DLayout(), vShaderByteCode));

			//Bind Topology
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			//Bind Transform
			AddBind(std::make_unique<TransformConstantBuffer>(this));

			SetIndexFromStatic();
		}
	}



	DirectX::XMMATRIX StaticMesh::GetTransformXM() const
	{
		return DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(m_Position.X, m_Position.Y, m_Position.Z) * //Move relative to origin
			DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f); //Rotate around world centre
	}
}