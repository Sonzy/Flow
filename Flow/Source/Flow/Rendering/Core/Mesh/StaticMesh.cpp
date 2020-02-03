#include "Flowpch.h"
#include "StaticMesh.h"
#include "Flow/Rendering/Core/Vertex/VertexLayout.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"

#include "Flow\Application.h"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include <random>


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
			const aiScene* Model = Importer.ReadFile(Application::GetApplication().GetLocalFilePath() + LocalPath,
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

			std::wstring Local = Application::GetApplication().GetLocalFilePathWide();
			//Bind Shaders
			auto vShader = std::make_unique<VertexShader>(Local + L"Flow/Source/Flow/Rendering/Core/Shaders/SolidColorVS.cso");
			auto vShaderByteCode = vShader->GetByteCode();
			AddStaticBindable(std::move(vShader));
			AddStaticBindable(std::make_unique<PixelShader>(Local + L"Flow/Source/Flow/Rendering/Core/Shaders/SolidColourPS.cso"));

			//Bind Index Buffer
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(indices));

			//Bind Input Layout
			AddStaticBindable(std::make_unique<InputLayout>(VBuffer.GetLayout().GetD3DLayout(), vShaderByteCode));

			//Bind Topology
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
			SetIndexFromStatic();
			
		//Bind Transform
		AddBind(std::make_unique<TransformConstantBuffer>(this));

		//Bind pixel constant buffer
		struct PSCBUF
		{
			DirectX::XMFLOAT4 Color;
		} Consts;

		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> Dist(0.0f, 1.0f);

		float r = Dist(rng);
		float g = Dist(rng);
		float b = Dist(rng);
		Consts.Color = { r, g, b, 1 };
		AddBind(std::make_unique<PixelConstantBuffer<PSCBUF>>(Consts, 0u));
	}



	DirectX::XMMATRIX StaticMesh::GetTransformXM() const
	{
		return DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.Pitch, m_Rotation.Yaw, m_Rotation.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(m_Position.X, m_Position.Y, m_Position.Z) * //Move relative to origin
			DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f); //Rotate around world centre
	}
}